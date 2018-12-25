#!/bin/bash


rm -rf argtable2-13
rm -rf Catch2-2.4.1
rm -rf hiredis-0.14.0
rm -rf libmicrohttpd-0.9.59
rm -rf curl-7.60.0
rm -rf jsoncpp-1.8.4
rm -rf libjson-rpc-cpp-1.1.1


tar -zxf argtable2-13.tar.gz
tar -zxf Catch2-2.4.1.tar.gz
tar -zxf hiredis-0.14.0.tar.gz
tar -zxf libmicrohttpd-0.9.59.tar.gz
tar -zxf curl-7.60.0.tar.gz
tar -zxf jsoncpp-1.8.4.tar.gz
tar -zxf libjson-rpc-cpp-1.1.1.tar.gz



echo ">>>>>>>>>>>>>>>>>>>> install argtable2"
cd argtable2-13

./configure
make
sudo make install

cd ..


echo ">>>>>>>>>>>>>>>>>>>> install Catch2"
cd Catch2-2.4.1

mkdir build
cd build
cmake ..
make
sudo make install
cd ..

cd ..


echo ">>>>>>>>>>>>>>>>>>>> install hiredis"
cd hiredis-0.14.0

make
sudo make install

cd ..


echo ">>>>>>>>>>>>>>>>>>>> install libmicrohttpd"
cd libmicrohttpd-0.9.59

./configure
make
sudo make install

cd ..



echo ">>>>>>>>>>>>>>>>>>>> install curl"
cd curl-7.60.0

./configure
make
sudo make install

cd ..


echo ">>>>>>>>>>>>>>>>>>>> install jsoncpp"
cd jsoncpp-1.8.4

sed '15 a set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")' -i CMakeLists.txt

mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install
cd ..

cd ..

echo ">>>>>>>>>>>>>>>>>>>> install libjson-rpc-cpp"
cd libjson-rpc-cpp-1.1.1


mkdir build
cd build
cmake ..
make
sudo make install
cd ..

cd ..

sudo updatedb
sudo ldconfig

exit 0


rm -rf argtable2-13
rm -rf Catch2-2.4.1
rm -rf hiredis-0.14.0
rm -rf libmicrohttpd-0.9.59
rm -rf curl-7.60.0
rm -rf jsoncpp-1.8.4
rm -rf libjson-rpc-cpp-1.1.1

