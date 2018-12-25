@echo off

protoc.exe -I=. --java_out=./ ./demo.proto


rem protoc.exe -I=. --grpc_out=./ --plugin=protoc-gen-grpc=grpc_cpp_plugin.exe ./helloworld.proto
rem protoc.exe -I=. --cpp_out=./ ./helloworld.proto
rem 
rem protoc.exe -I=. --grpc_out=./ --plugin=protoc-gen-grpc=grpc_cpp_plugin.exe ./callback.proto
rem protoc.exe -I=. --cpp_out=./ ./callback.proto


xcopy /Y /S prx E:\work\java\juzixproxyclient\src\main\java\prx\



protoc.exe -I=. --java_out=./ ./mpctype.proto
xcopy /Y /S net E:\work\java\juzixmpcvmsdk\src\main\java\net\
xcopy /Y /S net E:\work\java\juzixproxyclient\src\main\java\net\