# mpc vm platon sdk


## Dependencies

	juzixmpcvm_service


for platon call vm-service.


usage:

```go
package main

/*
put the dynamic library to the path that Golang can find it.
*/

/*
#cgo CFLAGS : -I.
#cgo LDFLAGS: -L. -ljuzixmpcvm_platonsdk

#include "platonvmsdk.h"
 */
import "C"
import "fmt"

func main() {
	var init_para C.init_para_st
	init_para.icecfg = C.CString("./ice.cfg")
	init_para.user = C.CString("user")
	init_para.pass = C.CString("pass")

	var vm_para C.vm_para_st
	vm_para.taskid = C.CString("hello-taskid")
	vm_para.pubkey = C.CString("hello-pubkey")
	vm_para.irhash = C.CString("hello-irhash")
	vm_para.method = C.CString("hello-method")
	vm_para.extras = C.CString("hello-extras")

	var pi C.pi_st
	pi.user = C.CString("user1")
	pi.pass = C.CString("user1")
	pi.host = C.CString("127.0.0.1")
	pi.port = 10001
	vm_para.pinfos[0] = pi

	pi.user = C.CString("user2")
	pi.pass = C.CString("user2")
	pi.host = C.CString("127.0.0.1")
	pi.port = 10002
	vm_para.pinfos[1] = pi

	ret := C.notify_security_calculation(vm_para, init_para)
	fmt.Print(ret)
}
```

The C header as follow:
```c

typedef struct init_para_st_ {
	char* icecfg;
	char* user;
	char* pass;
} init_para_st;

typedef struct peer_info_st_ {
	char* user;
	char* pass;
	char* host;
	int   port;
} pi_st;

typedef struct vm_para_st_ {
	char* taskid;
	char* pubkey;
	char* irhash;
	char* method;
	char* extras;
	pi_st pinfos[2]; // peers info, for 2PC, ONLY need 2 nodes
} vm_para_st;

/*
return 0 is success, other is failed
ERR_NO_ERROR	0
ERR_INIT_ENGINE	1
ERR_COMMIT_TASK	2
*/
#if __cplusplus
extern "C" {
#endif

	int PLATONMPCVMSDK_DLL_API notify_security_calculation(const vm_para_st vm_para, const init_para_st init_para);
	int PLATONMPCVMSDK_DLL_API notify_security_init(const init_para_st init_para);
	int PLATONMPCVMSDK_DLL_API notify_security_commit(const vm_para_st vm_para);

#if __cplusplus
}
#endif
```


