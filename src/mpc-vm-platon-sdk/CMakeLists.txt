cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_platonsdk)

set(NAME "mpc_vm_platonsdk")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)


###### includes here

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h ${CUR_DIR}/*.md)

# includes
include_directories(${ICE_ROOT}/include)
include_directories(${CUR_DIR}/sdk)

include_directories(${CUR_DIR}/../mpc-vm-common/common)
include_directories(${CUR_DIR}/../mpc-vm-common/utils)
include_directories(${CUR_DIR}/../mpc-vm-common/include)

include_directories(${CUR_DIR}/../mpc-vm-service/include)
include_directories(${CUR_DIR}/../mpc-vm-service/io)
include_directories(${CUR_DIR}/../mpc-vm-service/core)
include_directories(${CUR_DIR}/../mpc-vm-service/task)
include_directories(${CUR_DIR}/../mpc-vm-service/server)
include_directories(${CUR_DIR}/../mpc-vm-service/client)
include_directories(${CUR_DIR}/../mpc-vm-service/callback)
include_directories(${CUR_DIR}/../mpc-vm-service/jrpc)
include_directories(${CUR_DIR}/../mpc-vm-service/slice)

#source list
aux_source_directory(.      SRC_LIST)
aux_source_directory(./sdk  SRC_LIST)


###### libraries here
IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

link_libraries(mpc_vm_service)
link_libraries(mpc_vm_common_static)

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


###### build targets here
# dynamic
add_library(${NAME} SHARED ${SRC_LIST} ${juzixmpc_headers})

IF(WIN32)
    target_compile_options(${NAME} PRIVATE /D "PLATONMPCVMSDK_EXPORTS")
ENDIF()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
    FOLDER "mpc_vm"
	VERSION ${SO_VERSION}
)

# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${juzixmpc_headers})

IF(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "PLATONMPCVMSDK_STATIC")
ELSE()
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
ENDIF()

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
	FOLDER "mpc_vm"
	VERSION ${SO_VERSION}
)

#file(GLOB_RECURSE include_headers ${CUR_DIR}/include/*.h)
#INSTALL(FILES sdk/platonvmsdk.h DESTINATION ${CMAKE_PREFIX_PATH}/include/platonsdk)
install_libraries(${NAME} ${NAME}_static)


macro(compile_test testcase)
    # gflags
	include_directories(${CMAKE_BINARY_DIR}/gflags/include)
    link_directories(${CMAKE_BINARY_DIR}/gflags)
    link_libraries(gflags)

	set(projname mpc_vm_platonsdk-${testcase})
    add_executable(${projname} ./test/${testcase}.cpp ${CUR_DIR}/../mpc-vm-common/utils/command_parameters.cpp)
    target_link_libraries(${projname} ${NAME})
	
	SET_TARGET_PROPERTIES(${projname} PROPERTIES FOLDER "mpc_vm")
endmacro()

if(RUN_WITHOUT_PLATON)
    compile_test(platonvmsdk_main)
endif()


# demo, for test
add_library(mpc_vm_platonsdk_demo SHARED sdk-demo/platonvmsdk.cpp sdk/platonvmsdk.h)
SET_TARGET_PROPERTIES(mpc_vm_platonsdk_demo PROPERTIES FOLDER "mpc_vm")
