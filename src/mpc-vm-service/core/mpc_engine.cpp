#include <cstring>

#include <mutex>
#include <fstream>
using namespace std;

#include "utils2.h"
#include "type_utils.h"
#include "sys_utils_tool.h"
using namespace platon::mpc;

#include "mpc_engine.h"
#include "platon_logger.h"
#include "mpc_directnode_client.h"

#include "mpc_service_config.h"
#include "platon_mpc_service.h"

#include "ssl.h"
#include "libdevcore/RLP.h"

#include "jit.h"
#ifdef _WIN32
bool MyPlatON_InitJIT(const std::vector<std::string> &vectPluginName) {
    LOGI("windows not supported at present!");
    return true;
}
#else
#define MyPlatON_InitJIT PlatON_InitJIT
#endif

#ifdef ENABLE_ENCRYPT
#include <libdevcrypto/Common.h>
#endif

NS_PLATON_SDK_MPC_BEG


static recursive_mutex	 gs_locker;


MpcEngine::MpcEngine()
{
}


MpcEngine::~MpcEngine()
{
    LOGI("the engine in destruction !");
    destroy();
}

MpcEngine* MpcEngine::getInstance()
{
    static MpcEngine gs_engine;
    return &gs_engine;
}

void MpcEngine::unload()
{
    LOGW("to unload the mpc engine...");
    getInstance()->destroy();
}

void MpcEngine::destroy()
{
    LOGI("destroy...");
    std::lock_guard<std::recursive_mutex> lock(gs_locker);
    if (!m_inited) {
        LOGW("already destroy the engine");
        return;
    }

    if (m_mpcNodeServer2)
    {
        delete m_mpcNodeServer2;
        m_mpcNodeServer2 = nullptr;
    }
    LOGI("destroy node server ok.");

    m_taskDispatcher->destroy();
    if (m_taskDispatcher)
    {
        delete m_taskDispatcher;
        m_taskDispatcher = nullptr;
    }
    LOGI("destroy task dispatcher ok.");

    if (m_readyTaskProcessor) {
        delete m_readyTaskProcessor;
        m_readyTaskProcessor = nullptr;
    }
    LOGI("destroy ready task processor ok.");

    if (m_nodeClientFactory)
    {
        delete m_nodeClientFactory;
        m_nodeClientFactory = nullptr;
    }
    LOGI("destroy core MPC node client ok.");

    if (m_platonClient) {
        delete m_platonClient;
        m_platonClient = nullptr;
    }
    LOGI("destroy platon client ok.");

    m_inited = false;
    LOGI("destroy the engine ok");
}

int MpcEngine::init(
    const string& _iceCfg,
    const string& _user,
    const string& _password,
    platon::NodeCommunicateMode _mode,
    const string& _proxyEndpoint,
    const vector<string>& _args,
    const string& _platonUrl
) {
    m_user = _user;
    m_password = _password;
    m_mode = _mode;
    m_proxyEndpoint = _proxyEndpoint;
    m_args = _args;

    m_mpcDatDirectory = "";


    int step = 0;
    bool _nodeDirect = true;
    {
        LOGI("init step: %2d INIT MpcNodeServant", step++);
        m_mpcNodeServer2 = new MpcNodeServant();
        try {
            if (0 != m_mpcNodeServer2->init(_iceCfg, _user, _nodeDirect, _args)) {
                LOGE("node server init failed! config=%s", _iceCfg.data());
                return -1;
            }
        }
        catch (...)
        {
            LOGE("Got exception in init node server, config: %s", _iceCfg.data());
            return -1;
        }
    }

    {
        LOGI("init step: %2d INIT MpcTaskDispatcher", step++);
        m_taskDispatcher = new MpcTaskDispatcher();
        if (0 != m_taskDispatcher->init())
        {
            LOGE("INIT task dispatcher failed !");
            return -1;
        }
    }

    {
        LOGI("init step: %2d INIT MpcP2pTaskReadyProcessor", step++);
        m_readyTaskProcessor = new MpcP2pTaskReadyProcessor();
        if (0 != m_readyTaskProcessor->init(m_taskDispatcher->getProcessorNum()))
        {
            LOGE("INIT inputReadyTaskProcessor FAILED");
            return -1;
        }
    }

    {
        LOGI("init step: %2d INIT MpcNodeClientFactory", step++);
        m_nodeClientFactory = new MpcNodeClientFactory();
    }

    {
        LOGI("init step: %2d INIT PlatonClient", step++);
        m_platonClient = new PlatonClient(_platonUrl);
    }

    {
        LOGI("init step: %2d INIT PlatON JIT", step++);
        std::vector<std::string> libs{
            lib_mpc_vm_core_path,
            lib_protobuf_path
        };
        for (auto&lib : libs) {
            LOGI("INIT PlatON JIT lib: %s", lib.c_str());
        }
        if (!MyPlatON_InitJIT(libs)) {
            LOGE("INIT PlatON JIT failed !");
            return -1;
        }
    }
    {
        LOGI("init step: %2d INIT ABI FUNCTION", step++);
        int ret = dev::GetFunctionInfo(mpcc_cpp_abi_json_filepath, m_FuncInfos);
        if (ret != 0) {
            LOGE("INIT ABI FUNCTION FAILED!");
            return -1;
        }
        for (auto& fi : m_FuncInfos) {
            fi.print();
        }

        ret = dev::GetFunctionInfo(mpcc_cpp_abi_json_filepath, m_mapFuncInfos);
        if (ret != 0) {
            LOGE("INIT ABI FUNCTION FAILED!");
            return -1;
        }
    }

    {
        LOGI("init step: %2d INIT IR CACHE DATA DIR: %s", step++, ir_data_cache_dir);
        m_irDataDir = std::string(ir_data_cache_dir);
        if (!makedirs(m_irDataDir)) {
            LOGE("INIT IR CACHE DATA DIR FAILED!");
            return -1;
        }
    }

    LOGI("MPC ENGINE INIT SUCCESS");
    m_inited = true;
    return 0;
}

static std::string getString(const std::string& data) {
    int len = data.length();
    std::string tmpdata(data.substr(2, data.length() - 2));
    if (tmpdata.substr(62, 2) == "20") {
        std::string slen(tmpdata.substr(120, 8));
        unsigned char* buflen = new unsigned char[4];
        len = hexstr2buf(slen, buflen);
        int datalen = 0;
        datalen |= buflen[0] << 24;
        datalen |= buflen[1] << 16;
        datalen |= buflen[2] << 8;
        datalen |= buflen[3] << 0;
        datalen *= 2;
        std::cout << datalen << std::endl;
        return tmpdata.substr(128, datalen);
    }

    return "";
}

int MpcEngine::cacheIR(const std::string& ir_data, std::string& ir_hash, std::string& ir_path) {
    std::string s = getString(ir_data);
    if (s == "") {
        LOGE("IR DATA can not get string!");
        return -1;
    }
    int ilen = s.length() / 2 + 1;
    unsigned char* buf = new unsigned char[ilen];
    memset(buf, 0, ilen);
    int olen = hexstr2buf(s, buf);

    {
        std::string sok = std::string((char*)buf);
        delete[] buf;
        ilen = sok.length() / 2 + 1;
        buf = new unsigned char[ilen];
        memset(buf, 0, ilen);
        olen = hexstr2buf(sok, buf);
    }

    ir_hash = platon::mpc::SSL::md5(buf, olen);
    ir_path = m_irDataDir + "/" + ir_hash + ".data";
    LOGI("ir_hash %s", ir_hash.c_str());
    LOGI("ir_path %s", ir_path.c_str());

    if (m_irDataHashs.find(ir_hash) == m_irDataHashs.end()) {
        ofstream ofile(ir_path, ios::binary | ios::out);
        if (ofile.bad()) {
            LOGE("can not open ir path: %s ", ir_path.c_str());
            delete[] buf;
            return -1;
        }
        ofile.write((const char*)buf, olen);
        ofile.close();

        m_irDataHashs.insert(ir_hash);
    }
    delete[] buf;
    return 0;
}

int MpcEngine::getAllInfo(MPCTask& task) {
#define DEBUG_TEST
#if defined(ENABLE_ENCRYPT) && defined(DEBUG_TEST)
    dev::Public k("f5174b29d3ec35ece4752eeefaeb8aa1b875c3f1884ea8d84cce10422248d87dfbac11489232cba8623c2bb586ee4ccff4753c46bae5d64c37e6ece99273e384");
    std::vector<byte> bytes;

    std::vector<unsigned char> out = { 0x01,0x02,0x00,0x03 };
    int size = out.size();
    unsigned char* plain = new unsigned char[size];
    memset(plain, 0, size);
    int ilen = vectobuf(out, plain);
    dev::bytesConstRef plain_const(plain, ilen);
    dev::encryptECIES(k, plain_const, bytes);
    unsigned char* outbuf = new unsigned char[bytes.size()];
    int olen = vectobuf(bytes, outbuf);
    std::string enc_s = ToHexString(outbuf, olen);
    LOGI("enc_s %s", enc_s.c_str());

    dev::Secret sk("fbbc6a41063ff8abc15ca9bc77cb384b4e185ba27b0f5aa68cc6e35da87eec4a");
    std::vector<unsigned char> plain_text;
    dev::bytesConstRef cipher_const(outbuf, olen);
    dev::decryptECIES(sk, cipher_const, plain_text);
    unsigned char* outbuf2 = new unsigned char[bytes.size()];
    int olen2 = vectobuf(plain_text, outbuf2);
    std::string dec_s = ToHexString(outbuf2, olen2);
    LOGI("dec_s %s", dec_s.c_str());

#endif // for debug test
#undef DEBUG_TEST


    string address = m_mpcNodeServer2->m_taskSession->getAddress();
    if (address == "") {
        LOGW("have not register!");
        return -1;
    }
    TransParams transparams;
    transparams.from = address;
    transparams.to = task.iraddr;
    transparams.data = "";

    task.method_hash = platon::mpc::SSL::md5(task.method);
    LOGI("method_hash %s", task.method_hash.c_str());

    vector<byte> txtype = { 0,0,0,0,0,0,0,1 };
    // note: tag is only for test
    {
        //transparams.tag = "get_ir_data";
        transparams.data = dev::toHexPrefixed(dev::rlpList(txtype, m_mapFuncInfos["get_ir_data"].func_name));
        LOGI("transparams %s", transparams.toString().c_str());
        string ir_data = m_platonClient->get_ir_data(transparams);
        LOGI("ir data len: %d", ir_data.length());

        int len = ir_data.length();
        if (len < 131) {
            LOGE("GET IR DATA FAILED! len:%d", len);
            return -1;
        }

        if (0 != cacheIR(ir_data, task.irhash, task.irpath)) {
            LOGE("CACHE IR DATA FAILED!");
            return -1;
        }
    }

    {
        transparams.tag = "get_participants";
        transparams.data = dev::toHexPrefixed(dev::rlpList(txtype, m_mapFuncInfos["get_participants"].func_name));
        string ir_parties = m_platonClient->get_participants(transparams);
        LOGI("ir parties len: %d", ir_parties.length());
        {
            std::string s = getString(ir_parties);
            if (s == "") {
                LOGE("IR PARTIES can not get string!");
                return -1;
            }
            int ilen = s.length() / 2 + 1;
            unsigned char* buf = new unsigned char[ilen];
            memset(buf, 0, ilen);
            int olen = hexstr2buf(s, buf);
            ir_parties = std::string((char*)buf);
            LOGI("ir_parties: %s", ir_parties.c_str());
            delete[] buf;
        }
        task.participants = split(ir_parties, '&');
        if (task.participants.size()<2) {
            LOGE("participants size:%d", task.participants.size());
            return -1;
        }
    }

    {
        transparams.tag = "get_urls";
        transparams.data = dev::toHexPrefixed(dev::rlpList(txtype, m_mapFuncInfos["get_urls"].func_name));
        string ir_urls = m_platonClient->get_urls(transparams);
        LOGI("ir urls len: %d", ir_urls.length());
        {
            std::string s = getString(ir_urls);
            if (s == "") {
                LOGE("IR URLS can not get string!");
                return -1;
            }
            int ilen = s.length() / 2 + 1;
            unsigned char* buf = new unsigned char[ilen];
            memset(buf, 0, ilen);
            int olen = hexstr2buf(s, buf);
            ir_urls = std::string((char*)buf);
            LOGI("ir_urls: %s", ir_urls.c_str());
            delete[] buf;
        }
        task.urls = split(ir_urls, ',');
        for (auto url : task.urls) {
            auto au = split(url, '$');
            task.addr_url[au[0]] = au[1];
        }
        /// double check, not neccssary
        for (auto& au : task.addr_url) {
            // judge is registered
            if (m_mpcNodeServer2->m_taskSession->isRegistered(au.first)) {
                address = au.first;
                break;
            }
        }
        if (address == "") {
            LOGE("have not register!");
            return -1;
        }
        ///

        {
            vector<platon::NodeCommunicateInfo> nodeInfoList;
            for (auto& au : task.addr_url) {
                platon::NodeCommunicateInfo peerNodeinfo;
                peerNodeinfo.mode = ::platon::NodeCommunicateMode::MSERVICE;
                peerNodeinfo.user = au.first;
                peerNodeinfo.password = "";
                peerNodeinfo.endpoint = au.second;
                peerNodeinfo.ready = false;
                if (au.first == address) {
                    nodeInfoList.insert(nodeInfoList.begin(), peerNodeinfo); // 0, for self
                }
                else {
                    nodeInfoList.push_back(peerNodeinfo);
                }
            }
            if (nodeInfoList.size() == 0) {
                LOGE("peerNodeinfo is empty!");
                return -1;
            }
            task.setNodeInfoList(nodeInfoList);
        }
    }

    ///	{
    ///		transparams.tag = "get_url_by_id|" + address;
    ///		transparams.data = dev::toHexPrefixed(dev::rlpList(txtype, m_mapFuncInfos["get_url_by_id"].func_name, address));
    ///		string ir_url = m_platonClient->get_url_by_id(transparams, address.c_str());
    ///		std::cout << "ir_url:" << ir_url << std::endl;
    ///		{
    ///			std::string s = getString(ir_url);
    ///			unsigned char* buf = new unsigned char[s.length() / 2];
    ///			int olen = hexstr2buf(s, buf);
    ///			ir_url = std::string((char*)buf);
    ///			std::cout << "ir_url:" << ir_url << std::endl;
    ///			delete[] buf;
    ///		}
    ///	}

    {
        transparams.tag = "get_invitor";
        transparams.data = dev::toHexPrefixed(dev::rlpList(txtype, m_mapFuncInfos["get_invitor"].func_name));
        string ir_invitor = m_platonClient->get_invitor(transparams);
        LOGI("ir invitor len: %d", ir_invitor.length());
        {
            std::string s = getString(ir_invitor);
            if (s == "") {
                LOGE("IR INVITOR can not get string!");
                return -1;
            }
            int ilen = s.length() / 2 + 1;
            unsigned char* buf = new unsigned char[ilen];
            memset(buf, 0, ilen);
            int olen = hexstr2buf(s, buf);
            ir_invitor = std::string((char*)buf);
            LOGI("ir_invitor: %s", ir_invitor.c_str());
            delete[] buf;
        }
        if (ir_invitor == address) {
            task.invitor = true;
            task.role = platon::MPCRole::PART_ALICE;
        }
        else {
            task.invitor = false;
            task.role = platon::MPCRole::PART_BOB;
        }
    }
    ///	{
    ///		transparams.tag = "get_status|" + task.taskId;
    ///		transparams.data = "0x" + m_mapFuncInfos["get_status"].func_sha3.substr(0, 8);
    ///		uint64_t ir_status = m_platonClient->get_status(transparams, task.taskId.c_str());
    ///	}
    ///	{
    ///		transparams.tag = "get_fee|" + task.method;
    ///		transparams.data = "0x" + m_mapFuncInfos["get_fee"].func_sha3.substr(0, 8);
    ///		uint64_t ir_fee = m_platonClient->get_fee(transparams, task.method.c_str());
    ///	}

    return 0;

}

int MpcEngine::commit(MPCTask& task)
{
    if (!(m_taskDispatcher) || !m_inited) {
        LOGW("not INIT engine, cannot commit.");
        return -1;
    }

    if (0 != getAllInfo(task)) {
        LOGE("get ir info failed");
        return -1;
    }

    int ret = m_taskDispatcher->putCommitTask(task);
    if (ret != 0) {
        LOGE("commit a rsync task: %s failed !", task.taskId.data());
        return ret;
    }

    LOGI("commit a rsync task: %s success. to dispatcher.", task.taskId.data());
    return 0;
}


MpcNodeClient*  MpcEngine::getNodeClient(const platon::NodeCommunicateInfo& nodeinfo)
{
    if (!m_nodeClientFactory) {
        LOGW("not INIT engine, getNodeClient");
        return nullptr;
    }

    return m_nodeClientFactory->get(nodeinfo);
}

string MpcEngine::getMyEndpoint()
{
    if (!m_mpcNodeServer2)
    {
        LOGW("node server is not INIT !!!");
        return "";
    }

    return m_mpcNodeServer2->m_endpoint;
}


int MpcEngine::getWorkerNum()
{
    if (!m_taskDispatcher)
    {
        LOGW("not inited task dispatcher, cannot getWorkerNum");
        return -1;
    }

    return m_taskDispatcher->getProcessorNum();
}

NS_PLATON_SDK_MPC_END

