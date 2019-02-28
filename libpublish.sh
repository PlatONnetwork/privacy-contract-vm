#!/bin/bash

# please modify this script to fit your own first.


ROOT_DIR=/root/work
PLANG_DIR=${ROOT_DIR}/z/private-contract-compiler
JIT_DIR=${ROOT_DIR}/z/private-contract-compiler
VM_DIR=${ROOT_DIR}/y/private-contract-vm2

mpclibdir=${VM_DIR}/build/lib

rm -rf ./pub
mkdir -p ./pub

# generate by mpc project
libs+=${mpclibdir}/"libprotobufd.so "
libs+=${mpclibdir}/"libprotobuf-lited.so "
libs+=${mpclibdir}/"libmpc-jit.so "
libs+=${mpclibdir}/"libemp-tool.so "
libs+=${mpclibdir}/"libemp-tool.so.0.1.0 "
libs+=${mpclibdir}/"libmpc_vm_service.so "
libs+=${mpclibdir}/"libmpc_vm_service.so.0.1.0 "
libs+=${mpclibdir}/"libmpc_vm_platonsdk.so "
libs+=${mpclibdir}/"libmpc_vm_platonsdk.so.0.1.0 "

# deps
libs+="/opt/Ice-3.6.4/lib64/libIce++11.so.36 "
libs+="/opt/Ice-3.6.4/lib64/libIce++11.so.3.6.4 "
libs+="/opt/Ice-3.6.4/lib64/libGlacier2++11.so.36 "
libs+="/opt/Ice-3.6.4/lib64/libGlacier2++11.so.3.6.4 "
libs+="/opt/Ice-3.6.4/lib64/libIceUtil++11.so.36 "
libs+="/opt/Ice-3.6.4/lib64/libIceUtil++11.so.3.6.4 "

libs+="/usr/local/lib/libcurl.so.4 "
libs+="/usr/local/lib/libcurl.so.4.5.0 "
libs+="/usr/local/lib/libhiredis.so.0.14 "
libs+="/usr/local/lib/libmicrohttpd.so.12 "
libs+="/usr/local/lib/libmicrohttpd.so.12.46.0 "
libs+="/usr/local/lib64/libjsonrpccpp-common.so.1 "
libs+="/usr/local/lib64/libjsonrpccpp-common.so.1.1.1 "
libs+="/usr/local/lib64/libjsonrpccpp-client.so.1 "
libs+="/usr/local/lib64/libjsonrpccpp-client.so.1.1.1 "
libs+="/usr/local/lib64/libjsonrpccpp-server.so.1 "
libs+="/usr/local/lib64/libjsonrpccpp-server.so.1.1.1 "


for i in ${libs}; do
    echo ${i}
    cp -af ${i} ./pub/
done
