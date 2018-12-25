#include "mpc_task_processor.h"
#include <cstddef>

#include "utils2.h"
#include "type_utils.h"

#ifdef ENABLE_ENCRYPT
#include <libdevcrypto/Common.h>
#endif

#include "mpc_engine.h"
#include "platon_logger.h"
#include "simple_timer.h"
#include "communicator_io_channel.h"

#include "emp-sh2pc/semihonest.h"

using namespace platon::mpc;


#include "jit.h"
#ifdef _WIN32
int MyPlatON_RunFuncByLazyJIT(const char *pModuleName, const char *pFuncName,
    char **pRetBuf, char **argv, unsigned*unit_arg_len) {
    LOGI("windows not supported at present!");

    *pRetBuf = (char*)malloc(1);
    **pRetBuf = 1;

    return 1;
}
#else
#define MyPlatON_RunFuncByLazyJIT PlatON_RunFuncByLazyJIT
#endif

NS_PLATON_SDK_MPC_BEG

MpcTaskProcessor::MpcTaskProcessor(int id, int type)
    : m_id(id), m_atomicBufferSize(0), m_thread(nullptr), m_bufferSeq(0), m_type(type)
{

}

MpcTaskProcessor::~MpcTaskProcessor()
{
    if (m_thread)
    {
        destory();
    }
}


int MpcTaskProcessor::init(int max_queue)
{
    if (max_queue <= 0 || max_queue >= 1000000)
    {
        LOGE("init failed, bad max_queue: %d", max_queue);
        return -1;
    }

    if (m_running)
    {
        LOGW("already init the processor-%d, return", m_id);
        return 0;
    }

    //start the thread
    m_running = true;
    m_thread = new std::thread([&](MpcTaskProcessor* self) {
        LOGI("task processor: %d is running...", self->getID());
        while (self->running()) { // if m_running is false in the future ????
            self->processTask();
        }

        LOGW("task processor exit, terminate by outside !");
    }, this);

    m_requestSeq.store(0, std::memory_order_relaxed);
    m_bufferSeq.store(0, std::memory_order_relaxed);
    LOGI("INIT the processor: %d, type: %d ok", m_id, m_type);
    return 0;
}

void MpcTaskProcessor::destory()
{
    //stop the thread
    LOGI("to destroy processor-%d ...", m_id);

    m_running = false;

    if (m_thread)
    {
        m_cv.notify_all();
        m_thread->join();
        //release the resource
        delete m_thread;
        m_thread = nullptr;
    }

    std::lock_guard<std::mutex> lock(m_bufferMutex);
    m_taskBufferQue.clear();
    m_cv.notify_all();
    m_atomicBufferSize.store(0, std::memory_order_relaxed);

    LOGW("processor-%d destroyed .", m_id);
}

int MpcTaskProcessor::fetchTask(MPCTask& task)
{
    if (!m_running)
    {
        LOGW("the task processor is not inited !");
        return -1;
    }

    MpcTaskDispatcher* dispatcher = MpcEngine::getInstance()->getTaskDispatcher();
    if (0 != dispatcher->popReadyTask(task, m_type))
    {
        LOGW("POP a task failed ! may exit by outside user! we now set exit self !");
        m_running = false;
        return -1;
    }
    task.status = MPCTask::Status::POP_READY;

    LOGI("processor-%d fetch a task: %s, update the current task ", m_id, task.taskId.data());
    return 0;
}

const string MpcTaskProcessor::getTaskId()
{
    std::unique_lock<std::mutex> lock(m_bufferMutex);
    return m_task.getTaskId();
}


int MpcTaskProcessor::processTask()
{
    if (!m_running)
    {
        LOGW("the task processor is not inited !");
        return -1;
    }

    //get a task from dispacher ready queue
    MPCTask task;
    int ret = fetchTask(task);
    if (0 != ret)
    {
        LOGW("cannot fetch a task, break by outside!!");
        return ret;
    }

    //update the processing task
    {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        m_task = task;
    }

    m_bufferSeq.store(0, std::memory_order_relaxed);
    m_requestSeq.store(0, std::memory_order_relaxed);

    //start a mpc busi task (with a timer)
    LOGI("processor-%d (type: %d) to process a MPC task: %s ...", m_id, m_type, m_task.taskId.data());

    LOGI("PROCESS TASK BEG:\n%s\n", m_task.toString().c_str());
    ret = processMpcBusi(m_task);
    LOGI("PROCESS TASK END:\n%s\n", m_task.toString().c_str());

    if (0 != ret) {
        LOGW("processor a MPC task: %s Failed !", m_task.taskId.data());
        if (!m_task.invitor) {
            int ret = MpcEngine::getInstance()->getTaskDispatcher()->setFailedResult(m_task);
            LOGI("Bob set the failed result, task:%s, ret:%d", m_task.taskId.c_str(), ret);
        }
    }
    else {
        LOGI("processor-%d (type: %d) processed a MPC task: %s OK !", m_id, m_type, m_task.taskId.data());
    }

    // pop commit task
    auto taskDispatcher = MpcEngine::getInstance()->getTaskDispatcher();
    taskDispatcher->removeTaskState(m_task.taskId);
    auto  taskReadyProcessor = MpcEngine::getInstance()->getTaskReadyProcessor();
    taskReadyProcessor->removeRequest(m_task.taskId);

    return ret;
}

int MpcTaskProcessor::processMpcBusi(MPCTask& task)
{
    int step = 0; // only for debug, to tag the step in this function
    int ret = -1; // general return code
    std::string user_address;
    vector<unsigned char> out; // output data, return from "mpc-vm"
    vector<unsigned char> in; // input data, callback from "data provider"
    vector<unsigned char> in_x; // input data, for alice x means 02, for bob x means 01

    task.status = MPCTask::Status::PROCESS_INIT;

    ::platon::callback::MpcRequest request;
    {
        LOGI("process step:%02d assemble MpcRequest.\n", step++);
        request.customer = task.txaddr;
        request.task_id = task.taskId;
        request.peers = task.participants;
        request.method = task.method_hash;
        request.ir_hash = task.irhash;
        request.role = task.role;
    }
    {
        task.status = MPCTask::Status::PROCESS_SET_CALLBACK;
        LOGI("process step:%02d set data-provider callback", step++);
        try {
            auto mpcNodeServer2 = MpcEngine::getInstance()->getMpcNodeServant2();
            auto taskDispatcher = MpcEngine::getInstance()->getTaskDispatcher();
            user_address = (mpcNodeServer2->m_taskSession->getAddress());

            mpcNodeServer2->m_taskSession->setTransactionCB(user_address, task.transactionCB);
            mpcNodeServer2->m_taskSession->setTaskCB(user_address, task.method, task.taskCB);
        }
        catch (const std::exception& e) {
            LOGE("not ready, task:%s, error:%s", task.taskId.c_str(), e.what());
            return -1;
        }
    }
    {
        task.status = MPCTask::Status::PROCESS_GET_INPUT;
        LOGI("process step:%02d get input data by call data-provider inputData-callback-function.\n", step++);
        try {
            ::platon::ErrorEnum error = task.taskCB->inputData(request, in, in_x);
            if ((in.size() == 0) /*|| (in_x.size() == 0)*/ || (error != ::platon::ErrorEnum::NoError)) {
                LOGE("get input data, in size:%d, in_x size:%d, error:%d", in.size(), in_x.size(), static_cast<int>(error));
                return -1;
            }
            LOGI("get input data ok, in size:%d, in_x size:%d\n", in.size(), in_x.size());
        }
        catch (const Ice::ConnectionRefusedException& e) {
            LOGE("inputData Ice ConnectionRefusedException ice_name:%s, error:%d %s\n", e.ice_name().c_str(), e.error, e.what());
            return -1;
        }
        catch (const Ice::ConnectionLostException& e) {
            LOGE("inputData Ice ConnectionLostException ice_name:%s, error:%d %s\n", e.ice_name().c_str(), e.error, e.what());
            return -1;
        }
        catch (const Ice::Exception& e) {
            LOGE("inputData Ice Exception ice_name:%s, error:%s\n", e.ice_name().c_str(), e.what());
            return -1;
        }
    }

   emp::IOChannel *ioChannel = nullptr;
    {
        LOGI("process step:%02d set an io channel to mpc.", step++);
        ioChannel = new CommunicatorIOChannel(this);
        int party = static_cast<int>(request.role);// 1,ALICE 2,BOB
        LOGI("process step:%02d setup mpc compute, party:%d", step++, party);
        emp::setup_semi_honest(ioChannel, party);
    }

    {
        task.status = MPCTask::Status::PROCESS_RUN_FUNCTION;
        LOGI("process step:%02d call vm-core to start mpc-compute, and wait to return.", step++);

        char* result = nullptr;
        string ir_full_name = task.irpath;
        string ir_func_name = task.method;

        unsigned char* inputs[2] = { 0 };
        unsigned lengths[2] = { 0 };
        unsigned char* buf = new unsigned char[in.size() + 1];
        unsigned char* bufX = new unsigned char[in_x.size() + 1];
        memset(buf, 0, in.size() + 1);
        memset(bufX, 0, in_x.size() + 1);

        if (request.role == ::platon::MPCRole::PART_ALICE) {
            lengths[0] = vectobuf(in, buf);
            lengths[1] = vectobuf(in_x, bufX);
            inputs[0] = buf;
            inputs[1] = bufX;
        }
        else { // PART_BOB
            lengths[1] = vectobuf(in, buf);
            lengths[0] = vectobuf(in_x, bufX);
            inputs[1] = buf;
            inputs[0] = bufX;
        }

        {
            // for test, print inputs
            const char* szrole = (request.role == ::platon::MPCRole::PART_ALICE ? "ALICE" : "BOB");
            LOGI("role: %s, lengths[0]: %d, lengths[1]: %d", szrole, lengths[0], lengths[1]);
            for (int i = 0; i < 2; i++) {
                std::string s = ToHexString(inputs[i], lengths[i]);
                LOGI("inputs[%d]: %s", i, s.c_str());
            }
        }

        LOGI("------> mpc run PlatON_RunFuncByLazyJIT   ... task: %s, user: %s, role: %s", task.taskId.data(), user_address.data(), (request.role == ::platon::MPCRole::PART_ALICE ? "ALICE" : "BOB"));
        int result_len = MyPlatON_RunFuncByLazyJIT(ir_full_name.c_str(), ir_func_name.c_str(), &result, (char**)inputs, lengths);

        delete ioChannel;
        delete[] buf;
        delete[] bufX;

        if (result_len <= 0) {
            LOGE("call PlatON_RunFuncByLazyJIT, jit run failed, result_len %d.", result_len);
            free(result);
            return -1;
        }
        LOGI("call PlatON_RunFuncByLazyJIT, result[len:%d]:%s", result_len, ToHexString((unsigned char*)result, result_len).c_str());

        if (request.role == ::platon::MPCRole::PART_ALICE) {
            task.status = MPCTask::Status::PROCESS_OK;
            LOGI("ALICE return.");
            free(result);
            return 0;
        }

        buftovec(out, (const unsigned char*)result, result_len);
        free(result);
        LOGI("BOB go on.");
    }
    {
        task.status = MPCTask::Status::PROCESS_NOTIFY_RESULT;
        LOGI("process step:%02d notify the result(plaintext) to data-provider.\n", step++);
        try {
            task.taskCB->result(request, out);
        }
        catch (const Ice::Exception& e) {
            LOGE("result Ice Exception ice_name:%s, error:%s\n", e.ice_name().c_str(), e.what());
            //return -1;
        }
    }
    int olen = 0;
    unsigned char* outbuf = nullptr;
    {
        task.status = MPCTask::Status::PROCESS_ENCRYPT;
        LOGI("process step:%02d encrypt the result(plaintext) with pubkey.\n", step++);
#ifdef ENABLE_ENCRYPT
        dev::Public k(task.pubkey);
        std::vector<byte> bytes;

        int size = out.size();
        unsigned char* plain = new unsigned char[size];
        memset(plain, 0, size);
        int ilen = vectobuf(out, plain);
        dev::bytesConstRef plain_const(plain, ilen);
        dev::encryptECIES(k, plain_const, bytes);
        outbuf = new unsigned char[bytes.size()];
        olen = vectobuf(bytes, outbuf);

        std::string enc_s = ToHexString(outbuf, olen);
        LOGI("enc_s %s", enc_s.c_str());
#else
        outbuf = new unsigned char[out.size()];
        olen = vectobuf(out, outbuf);
#endif
        {
#define DEBUG_TEST // note: this test section only for show how to use sdk to decrypt encrypt-result
#if defined(ENABLE_ENCRYPT) && defined(DEBUG_TEST)
            dev::Public k("f5174b29d3ec35ece4752eeefaeb8aa1b875c3f1884ea8d84cce10422248d87dfbac11489232cba8623c2bb586ee4ccff4753c46bae5d64c37e6ece99273e384");
            std::vector<byte> bytes;

            int size = out.size();
            unsigned char* plain = new unsigned char[size];
            memset(plain, 0, size);
            int ilen = vectobuf(out, plain);
            dev::bytesConstRef plain_const(plain, ilen);
            dev::encryptECIES(k, plain_const, bytes);
            unsigned char* outbuf = new unsigned char[bytes.size()];
            int olen = vectobuf(bytes, outbuf);
            std::string enc_s = ToHexString(outbuf, olen);
            LOGI("task: %s enc_s %s", task.taskId.c_str(), enc_s.c_str());

            dev::Secret sk("fbbc6a41063ff8abc15ca9bc77cb384b4e185ba27b0f5aa68cc6e35da87eec4a");
            std::vector<unsigned char> plain_text;
            dev::bytesConstRef cipher_const(outbuf, olen);
            dev::decryptECIES(sk, cipher_const, plain_text);
            unsigned char* outbuf2 = new unsigned char[bytes.size()];
            int olen2 = vectobuf(plain_text, outbuf2);
            std::string dec_s = ToHexString(outbuf2, olen2);
            LOGI("task: %s dec_s %s", task.taskId.c_str(), dec_s.c_str());
#endif // for debug test
#undef DEBUG_TEST
        }
    }
    long nonce = 0L;
    {
        long tx_count = 0L;
        auto platonClient = MpcEngine::getInstance()->getPlatonClient();
        std::string s_count = platonClient->getTransactionCount(user_address, "pending");
        LOGI("process s_count:%s\n", s_count.c_str());
        MPC_IGNORE_EXCEPTIONS(tx_count = std::stol(s_count, nullptr, 0));
        long oldnonce = 0;
        nonce = tx_count;
        auto& userNonce = MpcEngine::getInstance()->getUserNonce();
        if (0 == userNonce.getElem(user_address, oldnonce)) {
            if (oldnonce > tx_count) {
                nonce = oldnonce + 1;
            }
        }
        userNonce.insert(user_address, nonce);
        LOGI("process step:%02d get tx_count: %ld nonce: %ld\n", step++, tx_count, nonce);
    }
    string sign_data = "";
    {
        task.status = MPCTask::Status::PROCESS_SIGN_TRANSACTION;
        LOGI("process step:%02d sign result(ciphertext) by call data-provider signTransaction2-callback-function.\n", step++);
        try {
            ::platon::callback::TransactionParams tps;
            {
                tps.task_id = task.taskId;
                tps.gas_price = 0L;
                tps.gas_limit = 0L;
                tps.nonce = nonce;
                tps.to_addr = task.iraddr;
                tps.state = 1; // 0 init, 1 sucess, other
                buftovec(tps.data, outbuf, olen);
            }
            sign_data = task.transactionCB->signTransaction2(tps);
            LOGI("sign_data:%s\n", sign_data.c_str());
            delete[] outbuf;
            outbuf = nullptr;
        }
        catch (const Ice::Exception& e) {
            delete[] outbuf;
            outbuf = nullptr;
            LOGE("signTransaction2 Ice Exception ice_name:%s error:%s\n", e.ice_name().c_str(), e.what());
            return -1;
        }
    }
    {
        task.status = MPCTask::Status::PROCESS_SEND_TRANSACTION;
        LOGI("process step:%02d send encrypted-signed-transaction to platon node.\n", step++);
        string params(sign_data);

        auto platonClient = MpcEngine::getInstance()->getPlatonClient();
        string transhash = platonClient->sendRawTransaction(params);
        if (transhash == "") {
            LOGE("sendRawTransaction failed");
            return -1;
        }
        task.setTransHash(transhash);
    }
    {
        task.status = MPCTask::Status::PROCESS_OK;
    }

    return 0;
}

void MpcTaskProcessor::inputBuffer(MpcBufferPtr buffer)
{
    //put a buffer
    {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        m_taskBufferQue[buffer->seq] = buffer;
        m_atomicBufferSize.fetch_add(1);
    }
    m_cv.notify_one();

    //LOGI("input buffer notify task: %s, seq: %d", buffer->taskId, buffer->seq);
}

int MpcTaskProcessor::consumeBuffer(MpcBufferPtr &buffer, int timeout)
{
    platon::mpc::SimpleTimer timer;
    //LOGI("to consume buffer,  task: %s ..... the next seq: %d", m_task.taskId.data(), m_requestSeq.load(std::memory_order_relaxed) + 1);

    //get buffer from the buffer queues
    {
        //first input buffer
        MpcTaskDispatcher* disp = MpcEngine::getInstance()->getTaskDispatcher();
        std::map<int, MpcBufferPtr> buffers;
        bool has_firstinput = false;
        if (disp->removeFirstInputBuffer(m_task.taskId, buffers))
        {
            has_firstinput = true;
        }

        std::unique_lock<std::mutex> lock(m_bufferMutex);
        for (std::map<int, MpcBufferPtr>::iterator it = buffers.begin(); it != buffers.end(); ++it)
        {//copy elements
            m_taskBufferQue.insert(*it);
        }

        while (m_running)
        {
            if (m_taskBufferQue.empty())
                m_cv.wait(lock);
            else if (m_taskBufferQue.begin()->second->seq != m_requestSeq.load(std::memory_order_relaxed) + 1)
            {
                m_cv.wait(lock);
            }
            else
                break;
        }
        if (!m_taskBufferQue.empty() && m_taskBufferQue.begin()->second->seq == m_requestSeq.load(std::memory_order_relaxed) + 1)
        {
            buffer = m_taskBufferQue.begin()->second;
            m_taskBufferQue.erase(buffer->seq);

            m_atomicBufferSize.fetch_sub(1);
            m_bufferSeq.fetch_add(1);//consume buffer seq, grow always
            m_requestSeq.fetch_add(1);//reqeust seq should be the same as the sender
            //LOGI("got buffer size: %d, task: %s, timeout: %d, cost time: %lf ", buffer->length, buffer->taskId, timeout, timer.stop());
            return 0;
        }
    }

    LOGW("got notify but not buffer to consume, task: %s, timeout: %d, cost time: %lf !!!!", m_task.taskId.data(), timeout, timer.stop());
    return -1;
}



int MpcTaskProcessor::getBufferCount()
{
    return m_atomicBufferSize.load(std::memory_order_relaxed);
}

NS_PLATON_SDK_MPC_END

