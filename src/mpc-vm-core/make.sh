cd build

rm CMake* -rf
rm cmake* -rf
rm Makefile
rm bin/add
rm bin/lib* -f

cmake -DCMAKE_BUILD_TYPE=$1 ..
make

cd ../

