#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_platonsdk)

set(NAME "mpc_vm_platonsdk")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

# find_package(OpenSSL)
#include_directories(${OPENSSL_INCLUDE_DIR})

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h)

# includes
include_directories(${ICE_ROOT}/include)
include_directories(${CUR_DIR}/transaction)


#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./transaction  SRC_LIST)


## print directory #####
# message("the include directory: ${INCLUDE_DIRECTORIES}" )
# message("the src dir: ${SRC_LIST}")


link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)

IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

link_libraries(Ice)

# dynamic
add_library(${NAME} SHARED ${SRC_LIST})

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm_test"
    )

# static
add_library(${NAME}_static STATIC ${SRC_LIST})

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm_test"
    )


INSTALL(TARGETS ${NAME} ${NAME}_static
    RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )

file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
INSTALL(FILES ${include_headers}        DESTINATION ${CMAKE_PREFIX_PATH}/include/platonsdk)

#install(FILES ${headers} DESTINATION include/${NAME})
install(TARGETS ${NAME} DESTINATION lib)

macro(compile_test testcase)
    add_executable(${testcase} ./test/${testcase}.cpp ${EXE_SRC_LIST})
    
    #target_link_libraries(${testcase} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} gmp)
    #INSTALL(TARGETS ${testcase} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endmacro()


#compile_test(testCommitTask)



