package main

/*
#cgo LDFLAGS: -Wl,-rpath="./libs"
#cgo LDFLAGS: -L./libs
#cgo LDFLAGS: -lmpc_vm_platonsdk
#include <stdio.h>
#include <stdlib.h>
#include "platonvmsdk.h"
*/
import "C"

import (
	"encoding/json"
	"fmt"
	"unsafe"
)
//const char* taskid, const char* pubkey, const char* address, const char* ir_address, const char* method, const char* extra
const (
	KEY_TASK_ID = "taskId"
	KEY_PUB_KEY = "pubKey"
	KEY_ADDRESS = "address"
	KEY_IR_ADDRESS = "irAddress"
	KEY_METHOD = "method"
	KEY_EXTRA = "extra"
)

var (
	icecfg = "./config/cfg.server1.config"
	url = "http://10.10.8.20:8545"
)

func init() {
	ccfg := C.CString(icecfg)
	cUrl := C.CString(url)
	defer func() {
		C.free(unsafe.Pointer(ccfg))
		C.free(unsafe.Pointer(cUrl))
	}()
	C.notify_security_init(ccfg, cUrl)
}

func Notify(jsonByt []byte) error {

	var jsonMap map[string]string
	err := json.Unmarshal(jsonByt, &jsonMap)
	if err != nil {
		//fmt.Println("json unmarshal fail: ", err.Error())
		return err
	}
	fmt.Println("Into Notify method, params :", jsonMap)
	// read key and convert to C attr.
	//const char* taskid, const char* pubkey, const char* address, const char* ir_address, const char* method, const char* extra
	cTaskId := C.CString(jsonMap[KEY_TASK_ID])
	cPubKey := C.CString(jsonMap[KEY_PUB_KEY])
	cAddress := C.CString(jsonMap[KEY_ADDRESS])
	cIRAddress := C.CString(jsonMap[KEY_IR_ADDRESS])
	cMethod := C.CString(jsonMap[KEY_METHOD])
	cExtra := C.CString(jsonMap[KEY_EXTRA])

	defer func() {
		C.free(unsafe.Pointer(cTaskId))
		C.free(unsafe.Pointer(cPubKey))
		C.free(unsafe.Pointer(cAddress))
		C.free(unsafe.Pointer(cIRAddress))
		C.free(unsafe.Pointer(cMethod))
		C.free(unsafe.Pointer(cExtra))
	}()

	C.notify_security_calculation(cTaskId, cPubKey, cAddress, cIRAddress, cMethod, cExtra)
	fmt.Println("Notify vm success")
	return nil
}
