#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_common)

set(NAME "mpc_vm_common")
message("the CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

IF(WIN32)
ELSE()
    set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
ENDIF()

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h ${CUR_DIR}/*.ice)


IF(OpenSSL_FOUND)
	message(STATUS "${OPENSSL_INCLUDE_DIR}")
	message(STATUS "${SSL_LIBEAY}")
	include_directories(${OPENSSL_INCLUDE_DIR})
	link_libraries(${OPENSSL_LIBRARIES})
ENDIF()


# includes
include_directories(${ICE_ROOT}/include)

include_directories(${CUR_DIR})
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/common)
include_directories(${CUR_DIR}/utils)

if(OT_NP_USE_MIRACL)
	include_directories(${CUR_DIR}/../../third-code/miracl/include)
endif()

#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./utils	SRC_LIST)


## print directory #####
#message("the include directory: ${INCLUDE_DIRECTORIES}" )
message("the src dir: ${SRC_LIST}" ${ICE_ROOT})


link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)

if(NOT OT_NP_USE_MIRACL)
	link_libraries(miracl)
endif()

#IF(WIN32)
#    link_libraries(Ws2_32)
#ENDIF()

#link_libraries(Ice)

# dynamic
#add_library(${NAME} SHARED ${SRC_LIST} ${juzixmpc_headers})
#SET_TARGET_PROPERTIES(${NAME} PROPERTIES
#    OUTPUT_NAME ${NAME}
#    FOLDER "mpc_vm"
#    )

# gflags
include_directories(${CMAKE_BINARY_DIR}/gflags/include)
link_directories(${CMAKE_BINARY_DIR}/gflags)
	
link_libraries(gflags)

# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${juzixmpc_headers})

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm"
    )

INSTALL(TARGETS ${NAME}_static
    RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )

file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
INSTALL(FILES ${include_headers}        DESTINATION ${CMAKE_PREFIX_PATH}/include/common)

#install(FILES ${headers} DESTINATION include/${NAME})
#install(TARGETS ${NAME} DESTINATION lib)

macro(compile_test testcase)
    add_executable(${testcase} ./test/${testcase}.cpp ${EXE_SRC_LIST})
    
    #target_link_libraries(${testcase} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} gmp)
    #INSTALL(TARGETS ${testcase} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endmacro()


#compile_test(testCommitTask)



