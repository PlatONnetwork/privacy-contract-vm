#!/bin/bash

find . -name '*.sh' | xargs chmod 755

#rm -rf build64
# 
# cd src/mpc-vm-service/slice
# . build.sh
# cd ../../../
# 
# echo "building x64 ..."
# mkdir build64
# cd build64
# cmake ..
# make
# make install
# cd ..


mkdir build
cd build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBOOST_INCLUDEDIR=/root/work/sources/boost_1_66_0 -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DOT_NP_USE_MIRACL=1
make
cd ..

mkdir build_without-platon
cd build_without-platon
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBOOST_INCLUDEDIR=/root/work/sources/boost_1_66_0 -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DOT_NP_USE_MIRACL=1 -DRUN_WITHOUT_PLATON=1
make
cd ..

mkdir build_relic
cd build_relic
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBOOST_INCLUDEDIR=/root/work/sources/boost_1_66_0 -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target
make
cd ..

mkdir build_relic_without-platon
cd build_relic_without-platon
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBOOST_INCLUDEDIR=/root/work/sources/boost_1_66_0 -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=target -DRUN_WITHOUT_PLATON=1
make
cd ..

