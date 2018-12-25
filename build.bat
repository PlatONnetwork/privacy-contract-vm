@echo off

REM rmdir /S /Q build64

cd src\mpc-vm-service\slice
call build.bat
cd ..\..\..\

cd src\mpc-vm-service\proto
call build.bat
cd ..\..\..\

echo "building x64 ..."
mkdir build64
cd build64
cmake .. -G"Visual Studio 15 2017 Win64" -DOPENSSL_ROOT_DIR=D:/PATH/openssl -DBOOST_INCLUDEDIR=D:/PATH/boost/include
cd ..


echo "building Win32 ..."
mkdir build32
cd build32
cmake .. -DOPENSSL_ROOT_DIR=D:/PATH/openssl
cd ..


rem pause 5 seconds
ping 127.0.0.1 -n 5 > nul
