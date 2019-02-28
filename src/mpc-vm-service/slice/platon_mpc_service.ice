
[["java:package:net.platon.vm.slice"]]

module platon {

    enum ErrorEnum {
        NoError             = 0,
        SystemError         = 100,
        ConnectionError     = 1001,
        UserPermissionError = 1002,
        TaskNotFound        = 1005,
        NoParticipant       = 1006,
        SomeOneOffline      = 1007,
        SomeOneRejectTask   = 1008,
        TaskExecTimeout     = 1011,
        TaskInputInvalid    = 1012,
        TaskExecError       = 1013,
    };

    enum MPCRole {
        PART_ALICE   = 1,
        PART_BOB     = 2,
        PART_GREEN   = 3,
        PART_NOBODY  = 4
    };

	enum NodeCommunicateMode {
		MCALLBACK,
		MSERVICE
	};

    struct NodeCommunicateInfo {
        NodeCommunicateMode mode;
        string user;
        string password;
        string endpoint;
		bool ready;
    };

	sequence<NodeCommunicateInfo> NodeCommunicateInfoList;
    sequence<byte> ByteList;
    sequence<string> StringList;
    sequence<ByteList> ByteLists;
    
    module callback {

        struct MpcRequest {
            string      customer;
            string      task_id;
            string      ir_hash;
            string      method; // hash
            MPCRole     role;
            StringList  peers; 
            int         index; // the position of mpc-function arguments, from 1 to n. eg. mpcfunc(x1,x2,x3,x4,x5,x6,...)
        };

        interface TaskCallback{
            /* 错误通知 */
            void error(MpcRequest request, ErrorEnum error);

            /* 通知输入数据 */
            ErrorEnum inputData(MpcRequest request, out ByteList data, out ByteList data_default);
            //ErrorEnum inputData2(MpcRequest request, out ByteLists data);

            /* 结果通知 */
            void result(MpcRequest request, ByteList data);
        };
        
		struct TransactionParams {
            string		task_id;
			long		gas_price;
			long		gas_limit;
			long		nonce;
			string		to_addr;
			ByteList	data;
			int			state;
		};

        interface TransactionCallback {
            /* 交易签名 */
            string signTransaction(string transaction);
            string signTransaction2(TransactionParams transparams);
        };

		interface NodeCallback {
			void invite(string taskid, int status);
			void notify(string taskid, int status);
			void ready(string taskid, int status);
		};
    };

    module service {

        interface TaskSession{
            /* 注册如存在，则覆盖 */
            ErrorEnum registTransactionCallback(string user_address, callback::TransactionCallback* cb);
            ErrorEnum registerIR(string ir_hash, callback::TaskCallback* cb);
        };
		
		struct TaskParams {
            string		taskid;
            string		pubkey;
            string		txaddr;
            string		iraddr;
            string		method;
			string		extras;
			string		irhash;
            MPCRole     role;
		};

        interface NodeChannelSession {
			void invite(TaskParams taskparams, callback::NodeCallback* cb); // alice -> bob
			void notify(TaskParams taskparams, callback::NodeCallback* cb); // bob -> alice

			int invite2(TaskParams taskparams); // alice -> bob, return status of the peer
			int notify2(TaskParams taskparams); // bob -> alice, return status of the peer

			void ready(TaskParams taskparams, callback::NodeCallback* cb);//NOTICE: the interface should delete

            /* 从对方读取输入数据 */
    		void inputData(string taskId, string fromUser, ByteList data);

            void input(ByteList data);
            void error(string taskId, MPCRole role, string from, ErrorEnum error);
        };
    };

};
