# clear cache
rm @@* -rf

#生成 test.ll:
clang++ testint.cpp -emit-llvm -S -O -fno-exceptions

#插入入口函数, 生成test1.ll：
#opt -load ./AddProtobufEntry.so -add-protobuf-entry testint.ll -S -o testint_all.ll
