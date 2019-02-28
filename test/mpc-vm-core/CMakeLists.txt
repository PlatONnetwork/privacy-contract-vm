cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_core_test)

set(NAME "mpc_vm_core_test")
set(JUZIX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src/mpc-vm-core)
GenOrganizedTree("" ${CMAKE_CURRENT_SOURCE_DIR} *.*)

ADD_DEFINITIONS(-D__USE_MM_INS__=1)

# includes here
include_directories(${JUZIX_SOURCE_DIR})
include_directories(${JUZIX_SOURCE_DIR}/include)
include_directories(../../src/mpc-vm-common/include)
include_directories(../../src/mpc-vm-common/utils)
include_directories(../../third-code/protobuf/src)
include_directories(../../third-code/jit/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/test)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/samples)

include_directories(${THIRD_CODE_DIR}/emp-ot)
include_directories(${THIRD_CODE_DIR}/emp-tool)
include_directories(${THIRD_CODE_DIR}/emp-tool/include)
include_directories(${THIRD_CODE_DIR}/emp-sh2pc)

#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./utils    SRC_LIST)


###### libraries here
IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

link_libraries(mpc_vm_common_static)
link_libraries(emp-tool)

link_ice() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_mpc_jit() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
support_jsonrpc() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_relic_or_miracl() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake

link_libraries(libdevcore-simplest)
IF(ENABLE_ENCRYPT)
    IF(WIN32)
	    link_directories(${WIN_THIRD_COMPILED_DIR}/cryptopp/lib/${PLATDIR})
    ELSE()
	    link_directories(${LINUX_THIRD_COMPILED_DIR}/cryptopp/lib)
    ENDIF()
    link_libraries(libdevcrypto-simplest)
ENDIF()


function(compile_test _category _name)
	message(STATUS "mpc vm test project name: ${_name}")

	include_directories(${JUZIX_SOURCE_DIR})
    aux_source_directory(./utils EXE_SRC_LIST)
    
    # gtest
    include_directories(${THIRD_CODE_DIR}/gtest/googletest/include)
	if (${CMAKE_BUILD_TYPE} MATCHES Debug)
		link_libraries(gtestd gtest_maind gmockd gmock_maind)
	else()
		link_libraries(gtest gtest_main gmock gmock_main)
	endif()


    SET(TEST_EXE_NAME mpc_test_${_category}_${_name})
    if (${_name} MATCHES TestProto)
	    include_directories(${THIRD_CODE_DIR}/protobuf/src)
        include_directories(./${_category})
        include_directories(./${_category}/code)
        add_executable(${TEST_EXE_NAME} ./${_category}/${_name}.cpp ${EXE_SRC_LIST} ./${_category}/code/${_name}.pb.cc)
	else()	
        add_executable(${TEST_EXE_NAME} ./${_category}/${_name}.cpp ${EXE_SRC_LIST})
	endif()
    
    SET_TARGET_PROPERTIES(${TEST_EXE_NAME} PROPERTIES OUTPUT_NAME ${TEST_EXE_NAME} FOLDER mpc_vm_test/mpc_vm_core/${_category})
    install_libraries(${TEST_EXE_NAME})
endfunction()


compile_test(samples millionaire)
compile_test(samples int2)
compile_test(samples Samples)
compile_test(samples SimpleProto1)

compile_test(test TestBasicType)
#compile_test(test TestProto)
compile_test(test TestPerf)

