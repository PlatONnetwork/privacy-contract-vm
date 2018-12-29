#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_service)

set(NAME "mpc_vm_service")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
GenOrganizedTree("" ${CUR_DIR} *.*)

#ADD_DEFINITIONS(-DICE_CPP11)#ADD_DEFINITIONS(-DJUZIX_OT_CACHE=1)
ADD_DEFINITIONS(-DJUZIX_OT_CACHE=1)

set(CMAKE_C_FLAGS "-pthread -march=native -O3 -maes -mrdseed")

file(GLOB_RECURSE mpc_vm_sdk_headers ${CUR_DIR}/*.h ${CUR_DIR}/*.ice ${CUR_DIR}/*.proto ${CUR_DIR}/*.json)

# includes
include_directories(${THIRD_CODE_DIR}/protobuf/src)
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/io)
include_directories(${CUR_DIR}/core)
include_directories(${CUR_DIR}/task)
include_directories(${CUR_DIR}/server)
include_directories(${CUR_DIR}/client)
include_directories(${CUR_DIR}/callback)
include_directories(${CUR_DIR}/jrpc)
include_directories(${CUR_DIR}/slice)

include_directories(${CUR_DIR}/test)
include_directories(${CUR_DIR}/testimpl)

include_directories(${CUR_DIR}/../mpc-vm-common/include)
include_directories(${CUR_DIR}/../mpc-vm-common/utils)

#include_directories(${CUR_DIR}/../mpc-vm-core)
#include_directories(${CUR_DIR}/../mpc-vm-core/include)
#include_directories(${CUR_DIR}/../mpc-vm-core/emp
include_directories(${THIRD_CODE_DIR}/emp-tool)
include_directories(${THIRD_CODE_DIR}/emp-tool/include)
include_directories(${THIRD_CODE_DIR}/emp-ot)
include_directories(${THIRD_CODE_DIR}/emp-sh2pc)

include_directories(${THIRD_CODE_DIR}/jit/include)
include_directories(${THIRD_CODE_DIR}/eth-simplest)


IF(NOT OT_NP_USE_MIRACL)
	message(STATUS "Use OT_NP_USE_MIRACL")
	message("CMAKE_BINARY_DIR is: ${CMAKE_BINARY_DIR}")
	
	find_package(OpenSSL) 
    IF(WIN32)
        SET(RELIC_INCLUDE_DIR  ${CMAKE_BINARY_DIR}/third-code/relic-win/include ${CUR_DIR}/../../third-code/relic-win/include ${CUR_DIR}/../../third-code/relic-win/include/low)
        set(RELIC_LIBRARIES relic_s)
    ELSE()
        set(RELIC_LIBRARIES relic_s)
        SET(RELIC_INCLUDE_DIR  ${CMAKE_BINARY_DIR}/third-code/relic/include ${CUR_DIR}/../../third-code/relic/include ${CUR_DIR}/../../third-code/relic/include/low)
    ENDIF()
	include_directories(${OPENSSL_INCLUDE_DIR})
	include_directories(${RELIC_INCLUDE_DIR})
ELSE()
	include_directories("${CUR_DIR}/../../third-code/miracl/include")
ENDIF()

include_directories(${ICE_ROOT}/include)


#source list
aux_source_directory(. SRC_LIST)
aux_source_directory(./task SRC_LIST)
aux_source_directory(./core SRC_LIST)
aux_source_directory(./jrpc SRC_LIST)
aux_source_directory(./client SRC_LIST)
aux_source_directory(./server SRC_LIST)
aux_source_directory(./callback SRC_LIST)
aux_source_directory(./io SRC_LIST)
aux_source_directory(./slice SRC_LIST)

aux_source_directory(./testimpl SRC_LIST)


link_directories(${CMAKE_BINARY_DIR}/lib)


IF(WIN32)
    IF(CMAKE_CL_64)
        set(PLATDIR x64)
    ELSE()
        set(PLATDIR Win32)
    ENDIF()
	include_directories(${WIN_THIRD_COMPILED_DIR}/libjson-rpc-cpp/include)
	link_directories(${WIN_THIRD_COMPILED_DIR}/libjson-rpc-cpp/lib/${PLATDIR})
	
	include_directories(${WIN_THIRD_COMPILED_DIR}/jsoncpp/include)
	link_directories(${WIN_THIRD_COMPILED_DIR}/jsoncpp/lib/${PLATDIR})
		
	include_directories(${WIN_THIRD_COMPILED_DIR}/curl/include)
	link_directories(${WIN_THIRD_COMPILED_DIR}/curl/lib/${PLATDIR})

	link_libraries(libcurl jsoncpp jsonrpccpp-common jsonrpccpp-client)
ELSE()
    find_package(CURL REQUIRED)
    find_package(Threads)
    message(STATUS "CURL header: ${CURL_INCLUDE_DIR}")
    message(STATUS "CURL lib   : ${CURL_LIBRARY}")
    
    find_package(MHD REQUIRED)
    message(STATUS "MHD header: ${MHD_INCLUDE_DIRS}")
    message(STATUS "MHD lib   : ${MHD_LIBRARIES}")
    
    find_package(Jsoncpp  REQUIRED)
    find_package(libjson-rpc-cpp  REQUIRED)
    
	#link_directories(/usr/local/lib /usr/lib /usr/lib64)
	link_directories(/usr/lib64/c++11)
	link_libraries(jsoncpp jsonrpccpp-common jsonrpccpp-client jsonrpccpp-server)
ENDIF()


if(OT_NP_USE_MIRACL)
	link_libraries(miracl)
endif()

IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

link_libraries(mpc_vm_common_static)

IF(WIN32)
	link_libraries(emp-tool_static)
ELSE()
	link_libraries(emp-tool)
	link_directories(${THIRD_CODE_DIR}/jit/lib)
	link_libraries(mpc-jit)
ENDIF()

IF(ENABLE_ENCRYPT)
IF(WIN32)
	link_directories(${WIN_THIRD_COMPILED_DIR}/cryptopp/lib/${PLATDIR})
ELSE()
	link_directories(${LINUX_THIRD_COMPILED_DIR}/cryptopp/lib)
ENDIF()
ENDIF()

link_libraries(libdevcore-simplest)
IF(ENABLE_ENCRYPT)
link_libraries(libdevcrypto-simplest)
ENDIF()

IF(OpenSSL_FOUND)
	message(STATUS "${OPENSSL_INCLUDE_DIR}")
	message(STATUS "${SSL_LIBEAY}")
	include_directories(${OPENSSL_INCLUDE_DIR})
	link_libraries(${OPENSSL_LIBRARIES})
ENDIF()



set(lib_plat "")
IF(CMAKE_CL_64)
    set(lib_plat "x64/")
ENDIF()

# link ice
IF(WIN32)
    link_directories(${ICE_ROOT}/lib/${lib_plat})
ELSE()
    link_directories(/usr/lib64/c++11 /usr/lib/x86_64-linux-gnu/c++11 ${ICE_ROOT}/lib/x86_64-linux-gnu/c++11 ${ICE_ROOT}/lib64/c++11)
ENDIF()

# dynamic
add_library(${NAME} SHARED ${SRC_LIST} ${mpc_vm_sdk_headers})
LinkIce(${NAME})

IF(WIN32)
    IF(CMAKE_CL_64)
        set(PLATDIR x64)
    ELSE()
        set(PLATDIR Win32)
    ENDIF()
	target_compile_options(${NAME} PRIVATE /D "PLATONMPCVM_SERVICE_EXPORTS")

	add_custom_command(TARGET ${NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${WIN_THIRD_COMPILED_DIR}/curl/lib/${PLATDIR}/$<CONFIGURATION>/libcurl.dll"
			$<TARGET_FILE_DIR:${NAME}>)
	INSTALL(FILES "${WIN_THIRD_COMPILED_DIR}/curl/lib/${PLATDIR}/$<CONFIGURATION>/libcurl.dll" DESTINATION ${CMAKE_PREFIX_PATH}/bin)
ENDIF()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
	FOLDER "mpc_vm"
)

add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/config/mpcc.cpp.abi.json"
		$<TARGET_FILE_DIR:${NAME}>)

# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${mpc_vm_sdk_headers})
LinkIce(${NAME}_static)

IF(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "PLATONMPCVM_SERVICE_STATIC")
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES FOLDER "mpc_vm")
ELSE()
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
ENDIF()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
	FOLDER "mpc_vm"
)

install_libraries(${NAME} ${NAME}_static)


function(mpc_jit_test _name)
	aux_source_directory(./test TEST_SRC_LIST)

#IF(WIN32)
#	link_libraries(emp-tool_static)
#ELSE()
		link_libraries(emp-tool)
		link_directories(${THIRD_CODE_DIR}/jit/lib)
		link_libraries(mpc-jit)
#	ENDIF()

	add_executable(mpc_jit_${_name} ${TEST_SRC_LIST})
	target_link_libraries(mpc_jit_${_name}  mpc-jit libprotobuf emp-tool)
endfunction()

#if(NOT WIN32)
	mpc_jit_test(millionaire)
#endif()