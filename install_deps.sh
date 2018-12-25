#!/bin/bash

####  1.  instlal packages by yum or apt install
Os=`awk -F= '/^NAME/{print $2}' /etc/os-release`
LinuxPlat=`echo ${Os} |tr -d '"'`
if [ "${LinuxPlat}" = "CentOS Linux" ]; then
        echo "the unix platform: ${LinuxPlat}"
        sudo yum install gcc g++ cmake git zlib-devel openssl-devel bzip2 expat-devel libdb-cxx-devel gmp-devel libcurl-devel boost-devel libcurl-devel.x86_64
elif [ "${LinuxPlat}" = "Ubuntu" ]; then
		echo "the unix platform: ${LinuxPlat}"
        sudo apt install gcc g++ cmake git zlib1g-dev libssl-dev bzip2 libexpat1-dev libdb++-dev libgmp-dev libcurl4-openssl-dev libboost-all-dev libcurl4-openssl-dev
else
        echo "the unix platform $LinuxPlat is not support at present !"
fi

####  2.  install packages that we already downloaded 
cd third-code/packages

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



echo ">>>>>>>>>>>>>>>>>>>> install cryptopp"
tar -zxf cryptopp-CRYPTOPP_5_6_5.tar.gz
cd cryptopp-CRYPTOPP_5_6_5
make -j4
sudo make install
cd ..

# install relic
cp ../relic ./ -R
cd relic
mkdir build && cd build && cmake -DALIGN=16 -DARCH=X64 -DARITH=curve2251-sse -DCHECK=off -DFB_POLYN=251 -DFB_METHD="INTEG;INTEG;QUICK;QUICK;QUICK;QUICK;LOWER;SLIDE;QUICK" -DFB_PRECO=on -DFB_SQRTF=off -DEB_METHD="PROJC;LODAH;COMBD;INTER" -DEC_METHD="CHAR2" -DCOMP="-O3 -funroll-loops -fomit-frame-pointer -march=native -msse4.2 -mpclmul" -DTIMER=CYCLE -DWITH="MD;DV;BN;FB;EB;EC" -DWSIZE=64 ..
make && make install
cd ..


# switch to base directory
cd ../../

# update index and config
sudo updatedb
sudo ldconfig

exit 0


