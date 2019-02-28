#!/bin/bash

ROOT_DIR=/root/work
PLANG_DIR=${ROOT_DIR}/z/private-contract-compiler
JIT_DIR=${ROOT_DIR}/z/private-contract-compiler
VM_DIR=${ROOT_DIR}/y/private-contract-vm2

cp -af ${JIT_DIR}/build/lib/libmpc-jit.so* ${VM_DIR}/third-code/jit/lib/


