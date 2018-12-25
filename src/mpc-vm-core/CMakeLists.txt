cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_core)

set(NAME "mpc_vm_core")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
GenOrganizedTree("" ${CUR_DIR} *.*)

#find_package(OpenSSL)
#include_directories(${OPENSSL_INCLUDE_DIR})

ADD_DEFINITIONS(-D__USE_MM_INS__=1)
#ADD_DEFINITIONS(-DJUZIX_OT_CACHE=1)
#ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)


if(NOT OT_NP_USE_MIRACL)
	message(STATUS "Use Relic")
	message("CMAKE_BINARY_DIR is: ${CMAKE_BINARY_DIR}")
	
	find_package(OpenSSL) 
    IF(WIN32)
        SET(RELIC_INCLUDE_DIR  ${CMAKE_BINARY_DIR}/third-code/relic-win/include ${CUR_DIR}/../../third-code/relic-win/include ${CUR_DIR}/../../third-code/relic-win/include/low)
        set(RELIC_LIBRARIES relic_s)
    ELSE()
        #find_package(relic)
        set(RELIC_LIBRARIES relic_s)
        SET(RELIC_INCLUDE_DIR  ${CMAKE_BINARY_DIR}/third-code/relic/include ${CUR_DIR}/../../third-code/relic/include ${CUR_DIR}/../../third-code/relic/include/low)
    ENDIF()
	include_directories(${OPENSSL_INCLUDE_DIR})
	include_directories(${RELIC_INCLUDE_DIR})
	#message(STATUS "OPENSSL_INCLUDE_DIR: ${OPENSSL_INCLUDE_DIR}")
	message(STATUS "RELIC_INCLUDE_DIR: ${RELIC_INCLUDE_DIR}")
else()
    ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)
	include_directories(${CUR_DIR}/../../third-code/miracl/include)
endif()


file(GLOB_RECURSE mpc_vm_core_headers ${CUR_DIR}/*.h)

# includes
if (${Protobuf_FOUND})
	include_directories(${Protobuf_INCLUDE_DIR})
else()
	include_directories(${THIRD_CODE_DIR}/protobuf/src)
endif()

include_directories(${CUR_DIR})
include_directories(${CUR_DIR}/../message)
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/../mpc-vm-common/include)
include_directories(${CUR_DIR}/../mpc-vm-common/utils)


# print info
#message("the src directory: ${CUR_DIR}")
#message("the include directory: ${INCLUDE_DIRECTORIES}" )


#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./emp-tool/io       SRC_LIST)
aux_source_directory(./emp-tool/gc       SRC_LIST)
aux_source_directory(./emp-tool/garble  SRC_LIST)
aux_source_directory(./emp-tool/circuits       SRC_LIST)
aux_source_directory(./emp-tool/utils    SRC_LIST)
aux_source_directory(./emp-tool/execution    SRC_LIST)
aux_source_directory(./emp-tool  SRC_LIST)
#aux_source_directory(../message  SRC_LIST)

aux_source_directory(./emp-ot  SRC_LIST)


##########3#####
# message("the include directory: ${INCLUDE_DIRECTORIES}" )
if(WIN32)
	link_directories(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
else()
	link_directories(${CMAKE_BINARY_DIR}/lib)
endif()

if(OT_NP_USE_MIRACL)
	link_libraries(miracl)
else()
    link_libraries(${RELIC_LIBRARIES})
endif()

IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

if(NOT WIN32)
    if (${CMAKE_BUILD_TYPE} MATCHES Debug)
    	link_libraries(protobufd)
    else()
    	link_libraries(protobuf)
    endif()
else()
    link_libraries(libprotobuf)
endif()

link_libraries(mpc_vm_common)


## dynamic
IF(NOT WIN32)
	message("-----  WIN32 BITES  dll")
	add_library(${NAME} SHARED ${SRC_LIST} ${mpc_vm_core_headers})
	INSTALL(TARGETS ${NAME}
		RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
		LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
		ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )
ENDIF()

#IF(WIN32)
#    target_compile_options(${NAME} PRIVATE /D "JUZIXMPCVM_CORE_EXPORTS")
#ENDIF()
#
#SET_TARGET_PROPERTIES(${NAME} PROPERTIES
#    OUTPUT_NAME ${NAME}
#    FOLDER "mpc_vm_core"
#    )

# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${mpc_vm_core_headers})
IF(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "JUZIXMPCVM_CORE_STATIC")
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES FOLDER "mpc_vm")
ELSE()
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
ENDIF()

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}_static
    FOLDER "mpc_vm"
	VERSION ${SO_VERSION}
)


# check relic and make target
if(NOT OT_NP_USE_MIRACL)
	message(STATUS "link relic,openssl library .")
	#target_link_libraries(${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES})
	target_link_libraries(${NAME}_static ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES})
else()
	message(STATUS "use miracl, not link relic,openssl library .")
	target_link_libraries(${NAME}_static)
	#target_link_libraries(${NAME}_static)
endif()

INSTALL(TARGETS ${NAME}_static
    RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )

file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
INSTALL(FILES ${include_headers}        DESTINATION ${CMAKE_PREFIX_PATH}/include/mpc_vm)

#install(FILES ${headers} DESTINATION include/${NAME})
#install(TARGETS ${NAME}_static DESTINATION lib)

macro(compile_test _name)
    #include_directories(${CMAKE_BINARY_DIR}/third-code/gflags/include)
    #link_directories(${CMAKE_BINARY_DIR}/third-code/gflags)
    #link_libraries(gflags)
    
    aux_source_directory(./testutils EXE_SRC_LIST)
    
    add_executable(${_name} ./test/${_name}.cpp ${EXE_SRC_LIST})
    
    if(NOT OT_NP_USE_MIRACL)
    	message(STATUS "the link libarary: ${NAME} , ${RELIC_LIBRARIES} ,  ${OPENSSL_LIBRARIES} , the target exe name: ${_name}")
        if(WIN32)
            target_link_libraries(${_name} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES})
        else()
            target_link_libraries(${_name} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} gmp)
        endif()
   	else()
   		message(STATUS "the link libarary: ${NAME} , the target exe name: ${_name}")
   		target_link_libraries(${_name} ${NAME})
   	endif()
    
    SET_TARGET_PROPERTIES(${_name} PROPERTIES FOLDER "mpc_vmTest")
    
    INSTALL(TARGETS ${_name} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endmacro()


#compile_test(mpctest)




