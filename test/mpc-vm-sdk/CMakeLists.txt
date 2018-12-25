#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_sdk_test)

set(NAME "mpc_vm_sdk_test")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

# find_package(OpenSSL)
#include_directories(${OPENSSL_INCLUDE_DIR})

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h)

# includes
include_directories(${ICE_ROOT}/include)
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/../mpc-vm-common/slice)
include_directories(${CUR_DIR}/api)
include_directories(${CUR_DIR}/test)


#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./include  SRC_LIST)
aux_source_directory(./api  SRC_LIST)


## print directory #####
# message("the include directory: ${INCLUDE_DIRECTORIES}" )
# message("the src dir: ${SRC_LIST}")


link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)

IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

link_libraries(mpc_vm_common mpc_vm_platonsdk)

# dynamic
add_library(${NAME} SHARED ${SRC_LIST})
SET_TARGET_PROPERTIES(${NAME} PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm"
    )

# static
add_library(${NAME}_static STATIC ${SRC_LIST})

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm"
    )


INSTALL(TARGETS ${NAME} ${NAME}_static
    RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )

file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
INSTALL(FILES ${include_headers}        DESTINATION ${CMAKE_PREFIX_PATH}/include/vmsdk)

#install(FILES ${headers} DESTINATION include/${NAME})
install(TARGETS ${NAME} DESTINATION lib)

macro(compile_test _name)
    #aux_source_directory(./testutils EXE_SRC_LIST)
    #add_executable(${_name} ./test/${_name}.cpp ${EXE_SRC_LIST})
    
    #target_link_libraries(${_name} ${NAME} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} gmp)
    #INSTALL(TARGETS ${_name} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endmacro()


#compile_test(testCommitTask)



