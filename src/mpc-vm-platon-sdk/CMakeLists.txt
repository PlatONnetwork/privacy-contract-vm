#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_platonsdk)

set(NAME "mpc_vm_platonsdk")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

# find_package(OpenSSL)
#include_directories(${OPENSSL_INCLUDE_DIR})

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h ${CUR_DIR}/*.md)

# includes
include_directories(${ICE_ROOT}/include)
include_directories(${CUR_DIR}/sdk)

include_directories(${CUR_DIR}/../mpc-vm-common/common)
include_directories(${CUR_DIR}/../mpc-vm-common/utils)
include_directories(${CUR_DIR}/../mpc-vm-common/include)

include_directories(${CUR_DIR}/../mpc-vm-service/utils)
include_directories(${CUR_DIR}/../mpc-vm-service/include)
include_directories(${CUR_DIR}/../mpc-vm-service/api)
include_directories(${CUR_DIR}/../mpc-vm-service/jrpc)
include_directories(${CUR_DIR}/../mpc-vm-service/core)
include_directories(${CUR_DIR}/../mpc-vm-service/task)
include_directories(${CUR_DIR}/../mpc-vm-service/busi)
include_directories(${CUR_DIR}/../mpc-vm-service/client)
include_directories(${CUR_DIR}/../mpc-vm-service/clientr)
include_directories(${CUR_DIR}/../mpc-vm-service/server)
include_directories(${CUR_DIR}/../mpc-vm-service/service)
include_directories(${CUR_DIR}/../mpc-vm-service/old)
include_directories(${CUR_DIR}/../mpc-vm-service/callback)
include_directories(${CUR_DIR}/../mpc-vm-service/communicator)
include_directories(${CUR_DIR}/../mpc-vm-service/entity)
include_directories(${CUR_DIR}/../mpc-vm-service/jni)
include_directories(${CUR_DIR}/../mpc-vm-service/slice)
include_directories(${CUR_DIR}/../mpc-vm-service/trans)
include_directories(${CUR_DIR}/../mpc-vm-service/mpc)

#include_directories(${CUR_DIR}/../juzix-mpc/include)
#include_directories(${CUR_DIR}/../juzix-mpc/mpc)
#include_directories(${CUR_DIR}/../juzix-mpc/label)
#include_directories(${CUR_DIR}/../juzix-mpc/utils)
#include_directories(${CUR_DIR}/../juzix-mpc/testutils)


#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./sdk  SRC_LIST)


## print directory #####
# message("the include directory: ${INCLUDE_DIRECTORIES}" )
# message("the src dir: ${SRC_LIST}")

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
ENDIF()

link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)

link_libraries(mpc_vm_service)
link_libraries(mpc_vm_common_static)

link_libraries(libdevcore-simplest)
IF(ENABLE_ENCRYPT)
link_libraries(libdevcrypto-simplest)
ENDIF()

IF(WIN32)
    link_libraries(Ws2_32)
ELSE()
    link_directories(${THIRD_CODE_DIR}/jit/lib)
    Link_libraries(mpc-jit)
ENDIF()

#IF(WIN32)
#	link_libraries(mpc_vm_core_static)
#ELSE()
#	link_libraries(mpc_vm_core)
#	link_directories(${THIRD_CODE_DIR}/jit/lib)
#	link_libraries(mpc-jit)
#ENDIF()

IF(ENABLE_ENCRYPT)
IF(WIN32)
	link_directories(${WIN_THIRD_COMPILED_DIR}/cryptopp/lib/${PLATDIR})
ELSE()
	link_directories(${LINUX_THIRD_COMPILED_DIR}/cryptopp//lib)
ENDIF()
ENDIF()

# link ice
IF(WIN32)
    link_directories(${ICE_ROOT}/lib/${lib_plat})
ELSE()
    link_directories(
    /usr/lib64/c++11
    /usr/lib/x86_64-linux-gnu/c++11
    ${ICE_ROOT}/lib/x86_64-linux-gnu/c++11
    ${ICE_ROOT}/lib64
    ${ICE_ROOT}/lib64/c++11
    )
ENDIF()

#link_libraries(Ice)

# dynamic
add_library(${NAME} SHARED ${SRC_LIST} ${juzixmpc_headers})

function(LinkIce projname)
    IF(WIN32)
        target_link_libraries(${projname} debug Iced)
        target_link_libraries(${projname} debug Glacier2d)
        target_link_libraries(${projname} debug IceUtild)
        target_link_libraries(${projname} optimized Ice)
        target_link_libraries(${projname} optimized Glacier2)
        target_link_libraries(${projname} optimized IceUtil)
    ELSE()
        target_link_libraries(${projname} Ice Glacier2 IceUtil)
    ENDIF()
endfunction()

LinkIce(${NAME})

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm"
    )
IF(WIN32)
target_compile_options(${NAME} PRIVATE /D "PLATONMPCVMSDK_EXPORTS")
ENDIF()


# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${juzixmpc_headers})
LinkIce(${NAME}_static)

IF(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "PLATONMPCVMSDK_STATIC")
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES FOLDER "mpc_vm")
ELSE()
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
ENDIF()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
	FOLDER "mpc_vm"
)

INSTALL(TARGETS ${NAME} ${NAME}_static
    RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )

file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
INSTALL(FILES sdk/platonvmsdk.h	DESTINATION ${CMAKE_PREFIX_PATH}/include/platonsdk)

#install(FILES ${headers} DESTINATION include/${NAME})
#install(TARGETS ${NAME} DESTINATION lib)

macro(compile_test testcase)
    # gflags
	include_directories(${CMAKE_BINARY_DIR}/gflags/include)
    link_directories(${CMAKE_BINARY_DIR}/gflags)
    link_libraries(gflags)

	set(projname mpc_vm_platonsdk-${testcase})
    add_executable(${projname} ./test/${testcase}.cpp
                            ${CUR_DIR}/../mpc-vm-common/utils/CommandParameters.cpp)
    
    target_link_libraries(${projname} ${NAME})
	
	SET_TARGET_PROPERTIES(${projname} PROPERTIES FOLDER "mpc_vm")
    #INSTALL(TARGETS ${testcase} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endmacro()


#compile_test(platonvmsdk_main)



# demo
add_library(mpc_vm_platonsdk_demo SHARED sdk-demo/platonvmsdk.cpp sdk/platonvmsdk.h)
SET_TARGET_PROPERTIES(mpc_vm_platonsdk_demo PROPERTIES FOLDER "mpc_vm")


