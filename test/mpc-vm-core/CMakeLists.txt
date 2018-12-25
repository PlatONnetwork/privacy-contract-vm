cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_core_test)

set(NAME "mpc_vm_core_test")
set(JUZIX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src/mpc-vm-core)
GenOrganizedTree("" ${CMAKE_CURRENT_SOURCE_DIR} *.*)


ADD_DEFINITIONS(-D__USE_MM_INS__=1)
ADD_DEFINITIONS(-DJUZIX_OT_CACHE=1)


if(OT_NP_USE_MIRACL)
	include_directories(../../third-code/miracl/include)
else()
	find_package(OpenSSL)
	include_directories(${OPENSSL_INCLUDE_DIR})

	if (WIN32)
		SET(RELIC_INCLUDE_DIR  ${CMAKE_BINARY_DIR}/third-code/relic-win/include ${CMAKE_BINARY_DIR}/third-code/relic-win/include/low)
	else()
		#find_package(relic)
        set(RELIC_LIBRARIES relic_s)
        SET(RELIC_INCLUDE_DIR  ${CUR_DIR}/../../third-code/relic/include ${CUR_DIR}/../../third-code/relic/include/low)
	endif()
endif()
include_directories(${RELIC_INCLUDE_DIR})


# includes
include_directories(${JUZIX_SOURCE_DIR})
include_directories(${JUZIX_SOURCE_DIR}/include)
include_directories(../../src/mpc-vm-common/include)
include_directories(../../src/mpc-vm-common/utils)
include_directories(../../third-code/protobuf/src)
include_directories(../../third-code/jit/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/unittest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/systest)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)


#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./utils       SRC_LIST)


##########3#####
message("the link director: ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}" )
# message("the src dir: ${SRC_LIST}")


if(WIN32)
	link_directories(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
    link_libraries(Ws2_32)
else()
	link_directories(${CMAKE_BINARY_DIR}/lib)
endif()

#add_library(${NAME} ${SRC_LIST} ${mpc_vm_core_test_headers})
#file(GLOB_RECURSE  include_headers      ${JUZIX_SOURCE_DIR}/*.h)

FUNCTION(LinkLib libname)
        link_libraries(debug ${libname}d)
        link_libraries(optimized ${libname})
ENDFUNCTION()

function(compile_test _category _name)
	include_directories(${JUZIX_SOURCE_DIR})
    aux_source_directory(./utils EXE_SRC_LIST)

	if(WIN32)
		link_directories(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
		link_libraries(Ws2_32)
	else()
		link_directories(${CMAKE_BINARY_DIR}/lib)
	endif()
	link_directories(${CMAKE_BINARY_DIR}/../third-code/jit/lib)

    #link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)
    #link_libraries(miracl mpc_vm_core)
    
    include_directories(${THIRD_CODE_DIR}/gtest/googletest/include)
	
	#LinkLib(libprotobuf)

	if(NOT WIN32)
		link_libraries(mpc-jit)
	endif()

	#	if(WIN32)
	#		if (${CMAKE_BUILD_TYPE} MATCHES Debug)
	#			SET(PROTO_BUF "libprotobufd")
	#			SET(PROTO_BUF_LITE "libprotobuf-lited")
	#		else()
	#			SET(PROTO_BUF "libprotobuf")
	#			SET(PROTO_BUF_LITE "libprotobuf-lite")
	#		endif()
	#	else()
	#		link_libraries(mpc-jit)
	#		if (${CMAKE_BUILD_TYPE} MATCHES Debug)
	#			SET(PROTO_BUF "protobufd")
	#			SET(PROTO_BUF_LITE "protobuf-lited")
	#		else()
	#			SET(PROTO_BUF "protobuf")
	#			SET(PROTO_BUF_LITE "protobuf-lite")
	#		endif()
	#	endif()

	if(NOT WIN32)
	link_libraries(mpc-jit)
	else()
	link_directories(${THIRD_CODE_DIR}/jit/lib/win/x64/Debug)
	link_libraries(mpc-jit)
	endif()

	link_libraries(libprotobuf)
	if (${CMAKE_BUILD_TYPE} MATCHES Debug)
		link_libraries(gtestd gtest_maind gmockd gmock_maind)
	else()	
		link_libraries(gtest gtest_main gmock gmock_main)
	endif()

    SET(TEST_EXE_NAME mpc_test_${_category}_${_name})
    #message("the test name: ${TEST_EXE_NAME}")
    add_executable(${TEST_EXE_NAME} ./${_category}/${_name}.cpp ${EXE_SRC_LIST})
    
    if(NOT OT_NP_USE_MIRACL)
    	#message(STATUS "the link libarary: ${NAME} , ${RELIC_LIBRARIES} ,  ${OPENSSL_LIBRARIES} , the target exe name: ${TEST_EXE_NAME}")
        if(WIN32)
            target_link_libraries(${TEST_EXE_NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} mpc_vm_core_static)
        else()
            target_link_libraries(${TEST_EXE_NAME} mpc_vm_core_static gmp)
        endif()
   	else()
   		#message(STATUS "the link libarary: ${NAME} , the target exe name: ${TEST_EXE_NAME}")
		if(WIN32)
   			target_link_libraries(${TEST_EXE_NAME} mpc_vm_core_static)
		else()
			target_link_libraries(${TEST_EXE_NAME} mpc_vm_core)
   		endif()
	endif()
    
    SET_TARGET_PROPERTIES(${TEST_EXE_NAME} PROPERTIES OUTPUT_NAME ${TEST_EXE_NAME} FOLDER mpc_vm_test/mpc_vm_core/${_category})
    #SET_TARGET_PROPERTIES(${TEST_EXE_NAME} PROPERTIES OUTPUT_NAME ${TEST_EXE_NAME} FOLDER "mpc_vm_core")

    INSTALL(TARGETS ${TEST_EXE_NAME} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endfunction()


# emp-tool basic
compile_test(unittest int)
compile_test(unittest bit)
compile_test(unittest garble)
compile_test(unittest netio)
compile_test(unittest prp)
compile_test(unittest pbutils)

# emp-ot test
compile_test(unittest shot)

# emp-sh2pc test
compile_test(systest bit)
compile_test(systest example)
compile_test(systest float)
compile_test(systest int)

# platon test cases
compile_test(systest simple)

