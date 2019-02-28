::@echo off


SET ROOT_DIR=E:\work\local
SET PLANG_DIR=%ROOT_DIR%\private-contract-compiler
SET JIT_DIR=%ROOT_DIR%\private-contract-compiler
SET VM_DIR=%ROOT_DIR%\private-contract-vm2

rmdir /S /Q pub
mkdir pub\x64\Debug\
mkdir pub\x64\Release\
mkdir pub\Win32\Debug\
mkdir pub\Win32\Release\



for %%i in (
    bzip2.dll
    emp-tool.dll
    gflags_nothreads.dll
    glacier236.dll
    gmock.dll
    gmock_main.dll
    gtest.dll
    gtest_main.dll
    ice36.dll
    iceutil36.dll
    libcurl.dll
    mpc_vm_platonsdk.dll
    mpc_vm_platonsdk_demo.dll
    mpc_vm_service.dll
    mpc-jit.dll
    libprotobuf.dll
    libprotobuf-lite.dll
) do (
    copy /Y %VM_DIR%\build\bin\Release\%%i   pub\x64\Release\
    copy /Y %VM_DIR%\build32\bin\Release\%%i pub\Win32\Release\
)



for %%i in (
    bzip2d.dll
    emp-tool.dll
    gflags_nothreads_debug.dll
    glacier236d.dll
    gmock_maind.dll
    gmockd.dll
    gtest_maind.dll
    gtestd.dll
    ice36d.dll
    iceutil36d.dll
    libcurl.dll
    mpc_vm_platonsdk.dll
    mpc_vm_platonsdk_demo.dll
    mpc_vm_service.dll
    mpc-jit.dll
    libprotobufd.dll
    libprotobuf-lited.dll
) do (
    copy /Y %VM_DIR%\build\bin\Debug\%%i   pub\x64\Debug\
    copy /Y %VM_DIR%\build32\bin\Debug\%%i pub\Win32\Debug\
)


rem pause 5 seconds
ping 127.0.0.1 -n 5 > nul
