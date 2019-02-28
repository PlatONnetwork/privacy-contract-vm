#!/bin/bash


rm -rf argtable2-13
rm -rf Catch2-2.4.1
rm -rf hiredis-0.14.0
rm -rf libmicrohttpd-0.9.59
rm -rf jsoncpp-1.8.4
rm -rf libjson-rpc-cpp-1.1.1


tar -zxf argtable2-13.tar.gz
tar -zxf Catch2-2.4.1.tar.gz
tar -zxf hiredis-0.14.0.tar.gz
tar -zxf libmicrohttpd-0.9.59.tar.gz
tar -zxf jsoncpp-1.8.4.tar.gz
tar -zxf libjson-rpc-cpp-1.1.1.tar.gz

curdir=`pwd`


cd ${curdir}
echo ">>>>>>>>>>>>>>>>>>>> install argtable2"
cd argtable2-13

./configure
make
sudo make install


cd ${curdir}
echo ">>>>>>>>>>>>>>>>>>>> install Catch2"
cd Catch2-2.4.1

mkdir build
cd build
cmake ..
make
sudo make install


cd ${curdir}
echo ">>>>>>>>>>>>>>>>>>>> install hiredis"
cd hiredis-0.14.0

make
sudo make install


cd ${curdir}
echo ">>>>>>>>>>>>>>>>>>>> install libmicrohttpd"
cd libmicrohttpd-0.9.59

./configure
make
sudo make install


cd ${curdir}
echo ">>>>>>>>>>>>>>>>>>>> install jsoncpp"
cd jsoncpp-1.8.4

sed '15 a set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")' -i CMakeLists.txt

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install


cd ${curdir}
echo ">>>>>>>>>>>>>>>>>>>> install libjson-rpc-cpp"
cd libjson-rpc-cpp-1.1.1


mkdir build
cd build
cmake ..
make
sudo make install


cd ${curdir}
sudo updatedb
sudo ldconfig


exit 0


rm -rf argtable2-13
rm -rf Catch2-2.4.1
rm -rf hiredis-0.14.0
rm -rf libmicrohttpd-0.9.59
rm -rf jsoncpp-1.8.4
rm -rf libjson-rpc-cpp-1.1.1

