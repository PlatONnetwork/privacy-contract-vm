#ifndef PLATON_MPCC_CPP
#define PLATON_MPCC_CPP

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <platon.hpp>


namespace mpc {
#define COMMON_SPLIT_CHAR "&"
#define OWNER "__OWNER__"

#define PREFIX "MPC_"
#define PREFIX_BONUS_RULE "BONUS_RULE_"

/// 定义存储变量的key信息
#define KEY_IR PREFIX "IR"
#define KEY_PARTIES PREFIX "PARTIES"
#define KEY_URLS PREFIX "URLS"
#define KEY_INVITORS PREFIX "INVITOR"
#define KEY_TASK_INFO PREFIX "TASK"

/// IR data -> $IR_VALUE_TEMPLATE$
#define IR_VALUE "0xf9885f880000000000000002b981010061736d010000000192818080001760000060037f7f7f017f60027e7f0060017f0060047f7f7f7f0060027f7f017f60027f7f0060017e0060017f017f60057f7e7e7e7e0060"
/// 发起者
#define INVITOR_VALUE "$INVITOR_VALUE_TEMPLATE$"
/// 参与者列表，多个使用 '&'分割，如：a&b&c -> $PARTIES_VALUE_TEMPLATE$
#define PARTIES_VALUE "0x60ceca9c1290ee56b98d4e160ef0453f7c40d219&0x60ceca9c1290ee56b98d4e160ef0453f7c40d211&0x60ceca9c1290ee56b98d4e160ef0453f7c40d212"
/// 参与者地址信息,多个使用 '&' 分割，断点信息携带对应用户标识
/// 此规则的定义与获取后的解码存在关系 -> $URLS_VALUE_TEMPLATE$
#define URLS_VALUE "0x60ceca9c1290ee56b98d4e160ef0453f7c40d219$tcpSession:tcp -p 9527,0x60ceca9c1290ee56b98d4e160ef0453f7c40d211$tcpSession:tcp -p 9527,0x60ceca9c1290ee56b98d4e160ef0453f7c40d212$tcpSession:tcp -p 9527"
/// 红利分配规则，字符串模拟k-v键值对
/// 格式：k1:v1,k2:v2,k3:v3 , 使用k-k-v模式进行存储
#define PROFIT_RULES_VALUE "$PROFIT_RULES_VALUE$"

/// 模拟k-k-v定义
#define PREFIX_RULES_MAP PREFIX "MAP_RULE_"
#define PREFIX_RESULT_MAP PREFIX "MAP_RESULT_"
#define PREFIX_ALLOT_MAP PREFIX "MAP_ALLOT_"

class MPC : public platon::Contract {
    public:
        PLATON_EVENT2(start_calc, uint64_t, const char *)
        PLATON_EVENT2(set_result, uint64_t, const char *)

        void init() {
            platon::setState(OWNER, platon::caller().toString()); // save msg.sender
            platon::setState(KEY_INVITORS, INVITOR_VALUE);      // save invitor
            platon::setState(KEY_PARTIES, PARTIES_VALUE);       // save partner
            platon::setState(KEY_URLS, URLS_VALUE);             // save all urls
            platon::setState(KEY_IR, IR_VALUE);                 // save ir data
            init_set_urls();                                    // save each one by id
        }

        const char * get_owner() const {
            std::string _owner_ ;
            platon::getState(OWNER, _owner_);
            return _owner_.c_str();
        }

		void fuc01(const char *extra) {
			start_calc("fuc01", nullptr);
		}
		
		void fuc02(const char *extra) {
			start_calc("fuc02", nullptr);
		}
		
		void fuc03(const char *method, const char *extra) {
			start_calc(const char *method, const char *extra);
		}
		
        void start_calc(const char *method, const char *extra) {

            // check msg.send is valid
            bool _isPartner = is_partner();
            if(!_isPartner){
                platon::println("Invalid msg.sender, not belong to partners.");
                PLATON_EMIT_EVENT2(start_calc, 0, "Invalid msg.sender, not belong to partners.");
                return;
            }

            // gen task id and save value
            const char *task_id = gen_task_id();
            platon::println("New task id: ", task_id);
            bool save_res = save_allot_amount(task_id);
            if(!save_res) {
                platon::println("save allot amount fail");
                PLATON_EMIT_EVENT2(start_calc, 0, "save allot amount fail");
                return;
            }

            // save task information. INFO_PREFIX + TASK_ID
            std::string task_key_str = KEY_TASK_INFO + std::string(task_id);
            std::string info_str = std::string(method) + COMMON_SPLIT_CHAR + std::string(extra);
            platon::setState(task_key_str, info_str);
            platon::println("start success..");
            PLATON_EMIT_EVENT2(start_calc, 1, task_id);
        }

        /// 设置计算结果，对质押的token按照费用扣除，扣除的费用在按照收益比例分配token，
        /// 结果将由参与者（Bob角色）计算完成设置
        void set_result(const char *taskId, uint64_t status, const char *data) {
            // check msg.send is valid
            bool _isPartner = is_partner();
            if(!_isPartner){
                platon::println("set_result->Invalid msg.sender, not belong to partners.");
                PLATON_EMIT_EVENT2(start_calc, 0, "Invalid msg.sender, not belong to partners.");
                return;
            }

            std::string key_str = PREFIX_RESULT_MAP + std::string(taskId);
            // check result set
            std::string res_value_str;
            platon::getState(key_str, res_value_str);
            if(res_value_str.length() > 1){
                platon::println("set_result-> set already. Can not reset again.");
                PLATON_EMIT_EVENT2(start_calc, 0, "set already. Can not reset again.");
                return;
            }

            // save result
            std::string status_str = toString(status);
            std::string data_str = std::string(data);
            std::string value_str = status_str + COMMON_SPLIT_CHAR + data_str;
            platon::println("save result, key: ", key_str, " value:", value_str);
            platon::setState(key_str, value_str);

            // divide the money. equal division.
            if(status != 1){    // 1 success 0 fail.
                platon::println("set_result-> status must equal 1.");
                PLATON_EMIT_EVENT2(start_calc, 0, "status must equal 1.");
                return;
            }
            uint64_t amount_i = get_saved_amount(taskId);
            const char *partners_c = get_participants();
            std::string partner_str = std::string(partners_c);
            std::vector<std::string> partner_vec = split(partner_str, COMMON_SPLIT_CHAR);
            uint64_t count_i = (uint64_t)partner_vec.size();
            platon::println("份数：", count_i);
            uint64_t each_one_i = amount_i / count_i;
            platon::println("avg ：", each_one_i);

            // transfer to partner
            for(unsigned int i = 0; i < partner_vec.size(); ++i) {
                std::string p_str = partner_vec[i];
                //platon::println("for each :", p_str);
                char *p_c = new char[42];
                strncpy(p_c, p_str.c_str(), 42);
                platon::println("transfer to:", std::string(p_c), " value:", each_one_i);
                ::callTransfer((platon::byte*)p_c, 42, each_one_i);
                delete []p_c;
            }
            PLATON_EMIT_EVENT2(set_result, 1, "Setting result success.");
        }

        const char * get_ir_data() const {
            std::string irdata;
            platon::getState(KEY_IR, irdata);
            return irdata.c_str();
        }

        const char * get_participants() const {
            std::string parties;
            platon::getState(KEY_PARTIES, parties);
            platon::println("get_participants:", parties);
            return parties.c_str();
        }
		
		/*
		* 获取任务的状态
		*/
		uint64_t get_status(const char* taskid) const {
			 //todo
			 return 0;
		}

		uint64_t get_fee(const char* method) const {
			//todo
			return 0;
		}
		
        const char * get_urls() const {
            std::string urls;
            platon::getState(KEY_URLS, urls);
            return urls.c_str();
        }

        const char * get_url_by_id(const char *address) const {
            std::string url;
            std::string key_str = KEY_URLS + std::string(address);
            platon::getState(key_str, url);
            return url.c_str();
        }

        const char * get_result(const char *task_id) const {
            std::string id_str = std::string(task_id);
            std::string key_str = PREFIX_RESULT_MAP + std::string(id_str);
            std::string result;
            platon::getState(key_str, result);
            return result.c_str();
        }

        const char * get_invitor() const {
            std::string invitor;
            platon::getState(KEY_INVITORS, invitor);
            return invitor.c_str();
        }

    private:
        const char * gen_task_id() const {
            uint64_t nonce_i = getCallerNonce();
            std::string nonce_str = toString(nonce_i);
            platon::h160 caller_h = platon::caller();
            std::string caller_h_str = caller_h.toString();
            platon::println("get nonce: ", nonce_str);
            std::string src_str = nonce_str + caller_h_str;
            char *t_src;
            strcpy(t_src, src_str.c_str());
            
            platon::h256 sha3_h = platon::sha3((platon::byte*)t_src, strlen(t_src));
            std::string sha3_h_str = sha3_h.toString();
            platon::println("task id : ", sha3_h_str);
            return sha3_h_str.c_str();
        }

        bool is_partner() {
            platon::h160 caller_h = platon::caller();
            std::string caller_h_str = caller_h.toString();
            std::string partner_str;
            platon::getState(KEY_PARTIES, partner_str);
            platon::println("参与者为：", partner_str);
            platon::println("调用者：", caller_h_str);
            std::string::size_type idx = caller_h_str.find("0x");
            if(idx == std::string::npos){
                caller_h_str = "0x" + caller_h_str;
            }
            platon::println("调用者end：", caller_h_str);
            std::vector<std::string> partner_vec = split(partner_str, COMMON_SPLIT_CHAR);
            for(unsigned int i = 0 ; i < partner_vec.size(); ++i) {
                platon::println("v_caller:", partner_vec[i]);
                if(strcmp(caller_h_str.c_str(), partner_vec[i].c_str()) == 0){
                    return true;
                }
            }
            return false;
        }

        /// 存储当次任务可用于代扣金额 value的值, wei
        bool save_allot_amount(const char *task_id) {
            uint64_t value_u = callValue();
            platon::println("value:", value_u);
            std::string key_str = PREFIX_ALLOT_MAP + std::string(task_id);
            std::string value_str = toString(value_u);
            if(value_u == 0) {
                value_str = "0";
            }
            platon::println("save_allot_amount: ", key_str, " - ", value_str);
            platon::setState(key_str, value_str);
            return true;
        }

        uint64_t get_saved_amount(const char *task_id) {
            std::string key_str = PREFIX_ALLOT_MAP + std::string(task_id);
            std::string value_str;
            platon::getState(key_str, value_str);
            platon::println("get_saved_amout: ", value_str);
            return stouint64(value_str);
        }

        void init_set_urls() {
            // URLS_VALUE -> "id01$tcpSession:tcp -p 9527,id02$tcpSession:tcp -p 9527,id03$tcpSession:tcp -p 9527"
            char *values = URLS_VALUE;
            char *pattern = ",";
            // ["id01$tcpSession:tcp -p 9527", "id02$tcpSession:tcp -p 9527", "id03$tcpSession:tcp -p 9527"]
            // KEY_URLS + "id01" -> tcpSession:tcp -p 9527
            platon::println("---- begin split ------");
            std::vector<std::string> arr = split(values, pattern);
            for(unsigned int i = 0 ; i < arr.size(); ++i) {
                char *trim_res = new char[100] ;
                char *str_in = new char[100];
                strcpy(str_in, arr[i].c_str());

                str_trim(str_in, trim_res);

                std::vector<std::string> id_valule_arr = split(str_in, "$");
                if(id_valule_arr.size() != 2) {
                    platon::println("Invalid value str of urls.");
                    platon::platonThrow("Invalid value str of urls.");
                }
                // do set state
                std::string __key = KEY_URLS + id_valule_arr[0];
                std::string __value = id_valule_arr[1];
                platon::println("init urls, key : ", __key, " , values: ", __value);
                platon::setState(__key, __value);
                delete []str_in;
                delete []trim_res;
            }
        }

    private:
        uint64_t stouint64(const std::string &num) {
            uint64_t res = 0;
            for (size_t i = 0; i < num.length(); i++) {
                res = res * 10 + (num[i] - '0');
            }
            platon::println("stouint64 ", num, "->", res);
            return res;
        }

        std::string toString(uint64_t num) const {
            std::string res;
            while (num != 0) {
                char c = num % 10 + '0';
                num /= 10;
                res.insert(0, 1, c);
            }
            platon::println("toString ", num, "->", res);
            return res;
        }

        std::vector <std::string> split(const std::string &str, const std::string &pattern) {
            // const char* convert to char*
            char *strc = new char[std::strlen(str.c_str()) + 1];
            strcpy(strc, str.c_str());
            std::vector <std::string> result_vec;
            char *tmp_str = strtok(strc, pattern.c_str());
            while (tmp_str != NULL) {
                result_vec.push_back(std::string(tmp_str));
                tmp_str = strtok(NULL, pattern.c_str());
            }
            delete[] strc;
            return result_vec;
        }

        void str_trim(char *str_in, char *str_out) {
            int i, j;
            i = 0;
            j = strlen(str_in) - 1;
            while(str_in[i] == ' '){
                i++;
            }
            while(str_in[j] == ' '){
                --j;
            }
            strncpy(str_out, str_in + i, j - i + 1);
            str_out[j - i + 1] = '\0';      // end flag
        }

    };

}

PLATON_ABI(mpc::MPC, fuc01);
PLATON_ABI(mpc::MPC, fuc02);
PLATON_ABI(mpc::MPC, fuc03);
PLATON_ABI(mpc::MPC, get_owner);
PLATON_ABI(mpc::MPC, set_result);
PLATON_ABI(mpc::MPC, get_ir_data);
PLATON_ABI(mpc::MPC, get_participants);
PLATON_ABI(mpc::MPC, get_urls);
PLATON_ABI(mpc::MPC, get_result);
PLATON_ABI(mpc::MPC, get_invitor);
PLATON_ABI(mpc::MPC, get_url_by_id);


#endif
//platon autogen begin
extern "C" { 
const char * get_owner() {
mpc::MPC MPC_platon;
return MPC_platon.get_owner();
}
void start_calc(const char * method,const char * extra) {
mpc::MPC MPC_platon;
MPC_platon.start_calc(method,extra);
}
void set_result(const char * taskId,unsigned long long status,const char * data) {
mpc::MPC MPC_platon;
MPC_platon.set_result(taskId,status,data);
}
const char * get_ir_data() {
mpc::MPC MPC_platon;
return MPC_platon.get_ir_data();
}
const char * get_participants() {
mpc::MPC MPC_platon;
return MPC_platon.get_participants();
}
const char * get_urls() {
mpc::MPC MPC_platon;
return MPC_platon.get_urls();
}
const char * get_url_by_id(const char * id) {
mpc::MPC MPC_platon;
return MPC_platon.get_url_by_id(id);
}
const char * get_result(const char * task_id) {
mpc::MPC MPC_platon;
return MPC_platon.get_result(task_id);
}
const char * get_invitor() {
mpc::MPC MPC_platon;
return MPC_platon.get_invitor();
}
void init() {
mpc::MPC MPC_platon;
MPC_platon.init();
}

}
//platon autogen end