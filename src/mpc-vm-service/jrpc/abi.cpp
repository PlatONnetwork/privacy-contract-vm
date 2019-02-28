#include "abi.h"

#include <string>
#include <fstream>
using namespace std;



#include "json/value.h"
#include "json/reader.h"

/// svr.config.json
/// {
///   "irhash": "475d728227601d5b4d3f4261f853c0a8",
///   "irpath": "SimpleAndProto.cpp.ll",
///   "invitor": "0x60ceca9c1290ee56b98d4e160ef0453f7c40d219",
///   "parties": "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba&0x28f9dbded3fca5e0c024401a1c0e4b1eb8673829",
///   "urls": "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba$DirectNodeServer:default -h localhost -p 10001,0x28f9dbded3fca5e0c024401a1c0e4b1eb8673829$DirectNodeServer:default -h localhost -p 10002"
/// }

struct ABIInit {
    ABIInit() {
#ifdef RUN_WITHOUT_PLATON
        getconfigjson();
        get_ir_data();
#endif
    }
    int getconfigjson()
    {
        Json::Reader reader;
        Json::Value root;

        std::ifstream is;
        is.open("svr.config.json", std::ios::binary);
        if (!is.good()) {
            cerr << "open svr.config.json failed!" << endl;
            exit(1);
        }
        if (reader.parse(is, root, false)) {
            irhash = root.get("irhash", "").asString();
            irpath = root.get("irpath", "").asString();
            invitor = root.get("invitor", "").asString();
            parties = root.get("parties", "").asString();
            urls = root.get("urls", "").asString();
        }
        is.close();

        return 0;
    }

    void get_ir_data() {

        ifstream ifile(irpath);
        if (!ifile.good()) {
            cerr << "open " << irpath << " failed!" << endl;
            exit(1);
        }
        while (!ifile.eof()) {
            char buf[4097] = { 0 };
            memset(buf, 0, 4096);
            ifile.read(buf, 4096);
            size_t count = ifile.gcount();
            buf[count] = 0;
            irdata += buf;
        }
        ifile.close();

    }

    std::string irhash = "";
    std::string irdata = "";
    std::string irpath = "";
    std::string invitor = "";
    std::string parties = "";
    std::string urls = "";
};
static ABIInit abiinit;


void set_result(const char *taskId, uint64_t status, const char *data) {
    std::cout << "set result" << "status" << status << " taskId: " << taskId << " data: " << data << std::endl;
}

const char * get_ir_hash() {
    return abiinit.irhash.c_str();
}
const char * get_ir_path() {
    return abiinit.irpath.c_str();
}
const char * get_ir_data() {
    return abiinit.irdata.c_str();
}
const char * get_participants() {
    return abiinit.parties.c_str();
    //return "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba&0x28f9dbded3fca5e0c024401a1c0e4b1eb8673829";
    //return "0x60ceca9c1290ee56b98d4e160ef0453f7c40d211&0x60ceca9c1290ee56b98d4e160ef0453f7c40d212";
}

const char * get_urls() {
    return abiinit.urls.c_str();
    //return "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba$DirectNodeServer:default -h localhost -p 10001,0x28f9dbded3fca5e0c024401a1c0e4b1eb8673829$DirectNodeServer:default -h localhost -p 10002";
    //return "0x60ceca9c1290ee56b98d4e160ef0453f7c40d211$DirectNodeServer:default -h localhost -p 10001,0x60ceca9c1290ee56b98d4e160ef0453f7c40d212$DirectNodeServer:default -h localhost -p 10002";
}

const char * get_url_by_id(const char *address) {
    map<string, string> m;
    m.insert({ "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba", "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba$DirectNodeServer:default -h localhost -p 10001" });
    m.insert({ "0x28f9dbded3fca5e0c024401a1c0e4b1eb8673829", "0x28f9dbded3fca5e0c024401a1c0e4b1eb8673829$DirectNodeServer:default -h localhost -p 10002" });
    //m.insert({ "0x60ceca9c1290ee56b98d4e160ef0453f7c40d211", "0x60ceca9c1290ee56b98d4e160ef0453f7c40d211$DirectNodeServer:default -h localhost -p 10001" });
    //m.insert({ "0x60ceca9c1290ee56b98d4e160ef0453f7c40d212", "0x60ceca9c1290ee56b98d4e160ef0453f7c40d212$DirectNodeServer:default -h localhost -p 10002" });
    return m[std::string(address)].c_str();
}

const char * get_invitor() {
    return abiinit.invitor.c_str();
    //return "0xee8ca8e7a8292ae978d93aa59f04f597ac3149ba";
    //return "0x60ceca9c1290ee56b98d4e160ef0453f7c40d211";
}

uint64_t get_status(const char* taskid) {
    return 0;
}

uint64_t get_fee(const char* method) {
    return 0;
}
