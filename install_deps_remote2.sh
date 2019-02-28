#!/bin/bash

# function echo ( ) {
    # echo -e "\033[32m "${1}" \033[0m"
# }

cmake_source_dir=$1
curdir=`pwd`

echo "Before install platon-mpc"


# if do not install dependencies
# exit 0


echo "install dependencies ..."


fakeroot rm -f /var/cache/apt/archives/lock
fakeroot rm -f /var/lib/dpkg/lock

echo "install apt-add-repository ..."
fakeroot apt-get install software-properties-common mlocate wget


depdir=/tmp/.mpc-deps
mkdir -p ${depdir}


function install_ice() {
    echo ">>>>>>>>>>>>>>>>>>>> install ice-all-dev"
    fakeroot apt-key adv --keyserver keyserver.ubuntu.com --recv 5E6DA83306132997
    fakeroot apt-add-repository "deb http://zeroc.com/download/Ice/3.6/ubuntu16.04 stable main"
    fakeroot apt-get update
    fakeroot apt-get install zeroc-ice-all-runtime zeroc-ice-all-dev
}
#install_ice

ret=`dpkg -l | grep zeroc-ice-all-dev | grep 3.6 > /dev/null`
if [ $? -ne 0 ]; then
    install_ice
fi

# /usr/local/lib/libargtable2.a
function install_libargtable2() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install argtable2"
    wget http://prdownloads.sourceforge.net/argtable/argtable2-13.tar.gz -O argtable2-13.tar.gz
    tar -zxf argtable2-13.tar.gz
    cd argtable2-13
    ./configure
    make
    fakeroot make install
}
#install_libargtable2


# only headers
function install_catch2() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install Catch2"
    wget https://github.com/catchorg/Catch2/archive/v2.4.1.tar.gz -O Catch2-2.4.1.tar.gz
    tar -zxf Catch2-2.4.1.tar.gz
    cd Catch2-2.4.1
    mkdir build
    cd build
    cmake ..
    make
    fakeroot make install
}
#install_catch2


# /usr/local/lib/libhiredis.so
function install_libhiredis() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install hiredis"
    wget https://github.com/redis/hiredis/archive/v0.14.0.tar.gz -O hiredis-0.14.0.tar.gz
    tar -zxf hiredis-0.14.0.tar.gz
    cd hiredis-0.14.0
    make
    fakeroot make install
}
#install_libhiredis



# /usr/local/lib/libmicrohttpd.so
function install_libmicrohttpd() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install libmicrohttpd"
    wget https://ftp.gnu.org/gnu/libmicrohttpd/libmicrohttpd-0.9.59.tar.gz -O libmicrohttpd-0.9.59.tar.gz
    tar -zxf libmicrohttpd-0.9.59.tar.gz
    cd libmicrohttpd-0.9.59
    ./configure
    make
    fakeroot make install
}
#install_libmicrohttpd


# /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1
function install_jsoncpp() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install jsoncpp"
    wget https://github.com/open-source-parsers/jsoncpp/archive/1.8.4.tar.gz -O jsoncpp-1.8.4.tar.gz
    tar -zxf jsoncpp-1.8.4.tar.gz
    cd jsoncpp-1.8.4

    sed '15 a set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")' -i CMakeLists.txt

    mkdir build
    cd build
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
    make
    fakeroot make install
}
#install_jsoncpp



# /usr/local/lib/libjsonrpccpp-common.so.1
function install_jsonrpc_cpp() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install libjson-rpc-cpp"
    wget https://github.com/cinemast/libjson-rpc-cpp/archive/v1.1.1.tar.gz -O libjson-rpc-cpp-1.1.1.tar.gz
    tar -zxf libjson-rpc-cpp-1.1.1.tar.gz
    cd libjson-rpc-cpp-1.1.1

    mkdir build
    cd build
    cmake ..
    make
    fakeroot make install
}
#install_jsonrpc_cpp

ret=`locate /usr/local/lib/libjsonrpccpp-common.so.1`
if [ $? -ne 0 ]; then
    install_libargtable2
    install_catch2
    
    ret=`locate /usr/local/lib/libhiredis.so`
    if [ $? -ne 0 ]; then
        install_libhiredis
    fi
    
    ret=`locate /usr/local/lib/libmicrohttpd.so`
    if [ $? -ne 0 ]; then
        install_libmicrohttpd
    fi

    ret=`locate /usr/lib/x86_64-linux-gnu/libjsoncpp.so.1`
    if [ $? -ne 0 ]; then
        install_jsoncpp
    fi
    
    install_jsonrpc_cpp
fi

# /usr/local/lib/libcryptopp.a
function install_cryptopp() {
    cd ${depdir}
    echo ">>>>>>>>>>>>>>>>>>>> install cryptopp"
    wget https://github.com/weidai11/cryptopp/archive/CRYPTOPP_5_6_5.tar.gz -O cryptopp-CRYPTOPP_5_6_5.tar.gz
    tar -zxf cryptopp-CRYPTOPP_5_6_5.tar.gz
    cd cryptopp-CRYPTOPP_5_6_5
    make -j4 CXXFLAGS="-DNDEBUG -g2 -O2 -fPIC -pipe"
    fakeroot make install
}
#install_cryptopp

ret=`locate /usr/local/lib/libcryptopp.a`
if [ $? -ne 0 ]; then
    install_cryptopp
fi


# /usr/local/lib/librelic.so
function install_relic() {
    cd ${cmake_source_dir}/third-code/relic
    rm -rf build && mkdir build && cd build && cmake -DALIGN=16 -DARCH=X64 -DARITH=curve2251-sse -DCHECK=off -DFB_POLYN=251 -DFB_METHD="INTEG;INTEG;QUICK;QUICK;QUICK;QUICK;LOWER;SLIDE;QUICK" -DFB_PRECO=on -DFB_SQRTF=off -DEB_METHD="PROJC;LODAH;COMBD;INTER" -DEC_METHD="CHAR2" -DCOMP="-O3 -funroll-loops -fomit-frame-pointer -march=native -msse4.2 -mpclmul" -DTIMER=CYCLE -DWITH="MD;DV;BN;FB;EB;EC" -DWSIZE=64 ..
    make
    fakeroot make install
}
#install_relic

ret=`locate /usr/local/lib/librelic.so`
if [ $? -ne 0 ]; then
    install_relic
fi


fakeroot updatedb
fakeroot ldconfig

cd ${curdir}
rm -rf ${depdir}


echo "install dependencies end"

