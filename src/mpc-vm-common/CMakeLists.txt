cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_common)

set(NAME "mpc_vm_common")
message("the CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

IF(NOT WIN32)
    set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
ENDIF()


###### includes here
include_directories(${ICE_ROOT}/include)

include_directories(${CUR_DIR})
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/common)
include_directories(${CUR_DIR}/utils)

# include current directory *.h
file(GLOB_RECURSE headers ${CUR_DIR}/*.h ${CUR_DIR}/*.ice)


# source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./utils	SRC_LIST)


###### libraries here
# gflags
include_directories(${CMAKE_BINARY_DIR}/gflags/include)
link_directories(${CMAKE_BINARY_DIR}/gflags)
link_libraries(gflags)

# openssl dependencies
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
link_libraries(${OPENSSL_LIBRARIES})


###### build targets here
# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${headers})
if(NOT WIN32)
    SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
endif()

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    FOLDER "mpc_vm"
	VERSION ${SO_VERSION}
    )

#file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
#INSTALL(FILES ${include_headers}        DESTINATION ${CMAKE_PREFIX_PATH}/include/common)
install_libraries(${NAME}_static)

