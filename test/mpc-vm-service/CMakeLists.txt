#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_service_test)

set(NAME "mpc_vm_service_test")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

# find_package(OpenSSL)
#include_directories(${OPENSSL_INCLUDE_DIR})

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h)

# includes

set(SERVICE_DIR ${CMAKE_SOURCE_DIR}/src/mpc-vm-service)
# includes
include_directories(${SERVICE_DIR}/utils)
include_directories(${SERVICE_DIR}/include)
include_directories(${SERVICE_DIR}/api)
include_directories(${SERVICE_DIR}/jrpc)
include_directories(${SERVICE_DIR}/core)
include_directories(${SERVICE_DIR}/task)
include_directories(${SERVICE_DIR}/busi)
include_directories(${SERVICE_DIR}/client)
include_directories(${SERVICE_DIR}/clientr)
include_directories(${SERVICE_DIR}/server)
include_directories(${SERVICE_DIR}/service)
include_directories(${SERVICE_DIR}/old)
include_directories(${SERVICE_DIR}/callback)
include_directories(${SERVICE_DIR}/communicator)
include_directories(${SERVICE_DIR}/entity)
include_directories(${SERVICE_DIR}/jni)
include_directories(${SERVICE_DIR}/slice)
include_directories(${SERVICE_DIR}/trans)
include_directories(${SERVICE_DIR}/mpc)
include_directories(${SERVICE_DIR}/test)
include_directories(${SERVICE_DIR}/testimpl)
include_directories(${SERVICE_DIR}/../mpc-vm-common/include)
include_directories(${SERVICE_DIR}/../mpc-vm-common/utils)
#include_directories(${SERVICE_DIR}/../juzix-mpc/include)
#include_directories(${SERVICE_DIR}/../juzix-mpc/mpc)
#include_directories(${SERVICE_DIR}/../juzix-mpc/label)
#include_directories(${SERVICE_DIR}/../juzix-mpc/utils)
#include_directories(${SERVICE_DIR}/../juzix-mpc/testutils)



#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./service  SRC_LIST)
aux_source_directory(./../mpc-vm-common/slice  SRC_LIST)


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


IF(WIN32)
    link_directories(${THIRD_CODE_DIR}/jit/lib/win/${PLATDIR}/Debug)
    link_directories(${THIRD_CODE_DIR}/jit/lib/win/${PLATDIR}/Release)
    link_libraries(mpc-jit)
ELSE()
	link_directories(${THIRD_CODE_DIR}/jit/lib)
	link_libraries(mpc-jit)
ENDIF()


IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

IF(OpenSSL_FOUND)
	message(STATUS "${OPENSSL_INCLUDE_DIR}")
	message(STATUS "${SSL_LIBEAY}")
	include_directories(${OPENSSL_INCLUDE_DIR})
	link_libraries(${SSL_LIBEAY})
ENDIF()

# link ice
IF(WIN32)
    link_directories(${ICE_ROOT}/lib/${lib_plat})
ELSE()
    link_directories(
    /usr/lib/x86_64-linux-gnu/c++11
    /opt/Ice-3.6.4/lib/x86_64-linux-gnu/c++11
    )
ENDIF()

#link_libraries(Ice mpc_vm_common)


function(compile_test _category _name)
    #include_directories(${CMAKE_BINARY_DIR}/third-code/gflags/include)
    #link_directories(${CMAKE_BINARY_DIR}/third-code/gflags)
    #link_libraries(gflags)

    include_directories(${include_headers})
    aux_source_directory(./utils EXE_SRC_LIST)

    link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)
    link_libraries(miracl)
    
	if(${_category} MATCHES "unittest")
		#gtest include and link
		include_directories(${CMAKE_BINARY_DIR}/../third-code/gtest/googletest-release-1.8.1/googletest/include)
		if (${CMAKE_BUILD_TYPE} MATCHES Debug)
			link_libraries(gtestd gtest_maind gmockd gmock_maind)
		else()	
			link_libraries(gtest gtest_main gmock gmock_main)
		endif()
	endif()

    add_executable(${_name} ./${_category}/${_name}.cpp ${EXE_SRC_LIST})
    
    if(JUZIX_OT_USE_RELIC)
    	message(STATUS "the link libarary: ${NAME} , ${RELIC_LIBRARIES} ,  ${OPENSSL_LIBRARIES} , the target exe name: ${_name}")
        if(WIN32)
            target_link_libraries(${_name} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES})
        else()
            target_link_libraries(${_name} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} gmp)
        endif()
   	else()
   		message(STATUS "the link libarary: ${NAME} , the target exe name: ${_name}")
   		target_link_libraries(${_name})
   	endif()
    
    SET_TARGET_PROPERTIES(${_name} PROPERTIES OUTPUT_NAME ${_name} FOLDER "mpc_vm_test/mpc-vm-service")
    
    INSTALL(TARGETS ${_name} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endfunction()


#compile_test(systest mpc_vm_sdk_test)

