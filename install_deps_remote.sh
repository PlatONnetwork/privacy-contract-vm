#!/bin/bash

curdir=`pwd`

mkdir ${curdir}/.deps

cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install argtable2"
wget http://prdownloads.sourceforge.net/argtable/argtable2-13.tar.gz -O argtable2-13.tar.gz
tar -zxf argtable2-13.tar.gz
cd argtable2-13
./configure
make
sudo make install


cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install Catch2"
wget https://github.com/catchorg/Catch2/archive/v2.4.1.tar.gz -O Catch2-2.4.1.tar.gz
tar -zxf Catch2-2.4.1.tar.gz
cd Catch2-2.4.1
mkdir build
cd build
cmake ..
make
sudo make install


cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install hiredis"
wget https://github.com/redis/hiredis/archive/v0.14.0.tar.gz -O hiredis-0.14.0.tar.gz
tar -zxf hiredis-0.14.0.tar.gz
cd hiredis-0.14.0

make
sudo make install


cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install libmicrohttpd"
wget https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.59.tar.gz -O libmicrohttpd-0.9.59.tar.gz
tar -zxf libmicrohttpd-0.9.59.tar.gz
cd libmicrohttpd-0.9.59
./configure
make
sudo make install


cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install jsoncpp"
wget https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz -O jsoncpp-1.8.4.tar.gz
tar -zxf jsoncpp-1.8.4.tar.gz
cd jsoncpp-1.8.4

sed '15 a set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")' -i CMakeLists.txt

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install


cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install libjson-rpc-cpp"
wget https://github.com/cinemast/libjson-rpc-cpp/archive/v1.1.1.tar.gz -O libjson-rpc-cpp-1.1.1.tar.gz
tar -zxf libjson-rpc-cpp-1.1.1.tar.gz
cd libjson-rpc-cpp-1.1.1


mkdir build
cd build
cmake ..
make
sudo make install


cd ${curdir}/.deps
echo ">>>>>>>>>>>>>>>>>>>> install cryptopp"
wget https://github.com/weidai11/cryptopp/archive/CRYPTOPP_5_6_5.tar.gz -O cryptopp-CRYPTOPP_5_6_5.tar.gz
tar -zxf cryptopp-CRYPTOPP_5_6_5.tar.gz
cd cryptopp-CRYPTOPP_5_6_5
make -j4
sudo make install


cd ${curdir}
sudo updatedb
sudo ldconfig
