@echo off

REM rmdir /S /Q build64
rem 
rem cd src\mpc-vm-service\slice
rem call build.bat
rem cd ..\..\..\
rem 
rem cd src\mpc-vm-service\proto
rem call build.bat
rem cd ..\..\..\
rem 


rem 
rem echo "building x64 ..."
rem mkdir build64
rem cd build64
rem cmake .. -G"Visual Studio 15 2017 Win64" -DOPENSSL_ROOT_DIR=D:/PATH/openssl -DBOOST_INCLUDEDIR=D:/PATH/boost/include
rem cd ..
rem 
rem 
rem echo "building Win32 ..."
rem mkdir build32
rem cd build32
rem cmake .. -DOPENSSL_ROOT_DIR=D:/PATH/openssl
rem cd ..
rem 


rem 64 bit
mkdir build
cd build
cmake .. -G"Visual Studio 15 2017 Win64" -DOPENSSL_ROOT_DIR=D:/PATH/openssl64 -DBOOST_INCLUDEDIR=D:/PATH/boost/include -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DPROJECT_NAME=platon-mpc-vm
cd ..

mkdir build_without-platon
cd build_without-platon
cmake .. -G"Visual Studio 15 2017 Win64" -DOPENSSL_ROOT_DIR=D:/PATH/openssl64 -DBOOST_INCLUDEDIR=D:/PATH/boost/include -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DRUN_WITHOUT_PLATON=1 -DPROJECT_NAME=platon-mpc-vm-without
cd ..

::exit 

mkdir build_relic
cd build_relic
cmake .. -G"Visual Studio 15 2017 Win64" -DOPENSSL_ROOT_DIR=D:/PATH/openssl64 -DBOOST_INCLUDEDIR=D:/PATH/boost/include -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DOT_NP_USE_RELIC_WIN=1 -DPROJECT_NAME=platon-mpc-vm-relic
cd ..

mkdir build_relic_without-platon
cd build_relic_without-platon
cmake .. -G"Visual Studio 15 2017 Win64" -DOPENSSL_ROOT_DIR=D:/PATH/openssl64 -DBOOST_INCLUDEDIR=D:/PATH/boost/include -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DOT_NP_USE_RELIC_WIN=1 -DRUN_WITHOUT_PLATON=1 -DPROJECT_NAME=platon-mpc-vm-relic-without
cd ..


rem 32 bit


rem pause 5 seconds
ping 127.0.0.1 -n 5 > nul
