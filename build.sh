#!/bin/bash

find . -name '*.sh' | xargs chmod 755

#rm -rf build64

cd src/mpc-vm-service/slice
. build.sh
cd ../../../

echo "building x64 ..."
mkdir build64
cd build64
cmake ..
make
make install
cd ..
