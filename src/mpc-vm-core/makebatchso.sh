#!/bin/bash
#make lib and test for Release or Debug, with gc batchsize: 4K, ..., 64M
#$1: Debug or $1, $2: 4K, 8K, ..., 256K, 1024K, 4096K, 64MB


function make_lib()
{
	rm CMake* -rf
	rm cmake* -rf
	rm Makefile
	#cmake -DCMAKE_BUILD_TYPE=$1 -DJUZIX_BATCH_SIZE_8K=1 ..
	cmake -DCMAKE_BUILD_TYPE=$1 "-DJUZIX_BATCH_SIZE_"$2=1 $3 ..
	make
}

function make_all()
{
	make_lib Debug 4K
	make_lib Debug 8K
	make_lib Debug 16K
	make_lib Debug 32K
	make_lib Debug 64K
	make_lib Debug 128K
	make_lib Debug 256K
	make_lib Debug 512K
	make_lib Debug 1M
	make_lib Debug 2M
	make_lib Debug 4M
	make_lib Debug 8M
	make_lib Debug 16M
	make_lib Debug 32M
	make_lib Debug 64M

	make_lib Release 4K
        make_lib Release 8K
        make_lib Release 16K
        make_lib Release 32K
        make_lib Release 64K
        make_lib Release 128K
        make_lib Release 256K
        make_lib Release 512K
        make_lib Release 1M
        make_lib Release 2M
        make_lib Release 4M
        make_lib Release 8M
        make_lib Release 16M
        make_lib Release 32M
        make_lib Release 64M
}

#change to build
cd build
if [ $1 = "all" ]; then
        echo "to make all libs..."
        make_all
else
	make_lib $1 $2 $3
fi

echo "---- make batch lib and exe version done ----"
cd ../

