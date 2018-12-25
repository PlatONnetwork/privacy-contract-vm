#!/bin/bash

#####################
##### On Ubuntu #####
#####################
find . -name '*.sh' | xargs chmod 755

curdir=`pwd`

sudo apt install gcc g++ cmake git zlib1g-dev libssl-dev bzip2 libexpat1-dev libdb++-dev libgmp-dev libcurl4-openssl-dev

sudo apt-get install autoconf automake libtool unzip tree



# Ice:3.6.4
mkdir deps

cd ${curdir}/deps
wget https://github.com/zeroc-ice/mcpp/archive/v2.7.2.12.tar.gz -O mcpp-2.7.2.12.tar.gz
tar -zxf mcpp-2.7.2.12.tar.gz
cd mcpp-2.7.2.12
make
sudo make install PREFIX=/usr/local

cd ${curdir}/deps
#https://github.com/zeroc-ice/ice/blob/3.6/cpp/BuildInstructionsLinux.md
#https://github.com/zeroc-ice/ice/blob/3.6/cpp/BuildInstructionsWindows.md
wget https://github.com/zeroc-ice/ice/archive/v3.6.4.tar.gz -O ice-3.6.4.tar.gz
tar -zxf ice-3.6.4.tar.gz
cd ice-3.6.4/cpp
sed '15 a CPP11 = yes' -i config/Make.rules
make
sudo make install

#relic
cd ${curdir}/deps
git clone https://github.com/relic-toolkit/relic.git
cd relic
cmake -DALIGN=16 -DARCH=X64 -DARITH=curve2251-sse -DCHECK=off -DFB_POLYN=251 -DFB_METHD="INTEG;INTEG;QUICK;QUICK;QUICK;QUICK;LOWER;SLIDE;QUICK" -DFB_PRECO=on -DFB_SQRTF=off -DEB_METHD="PROJC;LODAH;COMBD;INTER" -DEC_METHD="CHAR2" -DCOMP="-O3 -funroll-loops -fomit-frame-pointer -march=native -msse4.2 -mpclmul" -DTIMER=CYCLE -DWITH="MD;DV;BN;FB;EB;EC" -DWSIZE=64 .
make
sudo make install


cd ${curdir}/deps
cd third-code/z-linux-deps
install_deps.sh


cd ${curdir}
grep "export ICE_ROOT=/opt/Ice-3.6.4" /etc/profile || echo "export ICE_ROOT=/opt/Ice-3.6.4" >> /etc/profile

cd ${curdir}
rm -rf build64
mkdir build64
cd build64
cmake ..
make
make install

cd ${curdir}

echo "set JAVA_HOME & ICE_ROOT"
echo "export JAVA_HOME=/path/to/jdk1.8"
echo "export ICE_ROOT=/opt/Ice-3.6.4"


###
# java maven golang
# 
# https://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html
# https://archive.apache.org/dist/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz
# https://studygolang.com/dl/golang/go1.11.linux-amd64.tar.gz
# 
# HISTSIZE=100001
# 
# export ICE_ROOT=/opt/Ice-3.6.4
# export PATH=$ICE_ROOT/bin:$PATH
# 
# export JAVA_HOME=/usr/local/java/jdk1.8.0_191
# export PATH=$JAVA_HOME/bin:$PATH
# 
# export MAVEN_HOME=/usr/local/java/apache-maven-3.3.9
# export PATH=$MAVEN_HOME/bin:$PATH
# 
# export GOPATH=/root/work/gopath
# export GOROOT=/usr/local/go
# export PATH=$GOROOT/bin:$PATH
# 
# 
# llvm clang
# http://releases.llvm.org/6.0.0/llvm-6.0.0.src.tar.xz
# http://releases.llvm.org/6.0.0/cfe-6.0.0.src.tar.xz
# 
# 
# wget https://github.com/protocolbuffers/protobuf/archive/v3.6.1.tar.gz -O protobuf-3.6.1.tar.gz
# sudo apt install libboost-all-dev
# 
# 
# 

