cmake_minimum_required (VERSION 3.5)
project (plat-mpc-vm)
set(NAME "plat-mpc-vm")

message(STATUS "CMAKE_PROJECT_VERSION      : ${CMAKE_PROJECT_VERSION}")
message(STATUS "CMAKE_PROJECT_VERSION_MAJOR: ${CMAKE_PROJECT_VERSION_MAJOR}")
message(STATUS "CMAKE_PROJECT_VERSION_MINOR: ${CMAKE_PROJECT_VERSION_MINOR}")
message(STATUS "CMAKE_PROJECT_VERSION_PATCH: ${CMAKE_PROJECT_VERSION_PATCH}")

# setup directory where we should look for cmake files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

message(STATUS "CMAKE_BUILD_TYPE " ${CMAKE_BUILD_TYPE})
message(STATUS "JUZIX_BATCH_SIZE " ${JUZIX_BATCH_SIZE})

IF(WIN32)
SET(ENABLE_ENCRYPT TRUE)
ELSE()
SET(ENABLE_ENCRYPT TRUE)
ENDIF()
IF(ENABLE_ENCRYPT)
ADD_DEFINITIONS(-DENABLE_ENCRYPT)
ENDIF()

set(MAJOR_VERSION   1)
set(MINOR_VERSION   0)
set(PATCH_VERSION   0)
set(SO_VERSION      1)
set(SDK_VERSION     1)

message(STATUS "MAJOR_VERSION: ${MAJOR_VERSION}")
message(STATUS "MINOR_VERSION: ${MINOR_VERSION}")
message(STATUS "PATCH_VERSION: ${PATCH_VERSION}")
message(STATUS "SO_VERSION   : ${SO_VERSION}")
message(STATUS "SDK_VERSION  : ${SDK_VERSION}")

SET(MPCC_CPP_ABI_JSON_FILEDIR ./config)
configure_file (
    "${CMAKE_SOURCE_DIR}/config/mpc_service_config.h.in"
    "${CMAKE_BINARY_DIR}/include/mpc_service_config.h"
)
include_directories(${PROJECT_BINARY_DIR}/include)

SET(THIRD_CODE_DIR ${CMAKE_SOURCE_DIR}/third-code)
SET(WIN_THIRD_COMPILED_DIR ${THIRD_CODE_DIR}/z-windows-compiled)
SET(LINUX_THIRD_COMPILED_DIR ${THIRD_CODE_DIR}/z-linux-compiled)
SET(BOOST_INCLUDE ${THIRD_CODE_DIR}/boost_1_66_0)

# find packages
find_package(Git)
find_package(Boost)
find_package(OpenSSL)
find_package(ZLIB)
find_package(Protobuf)
MESSAGE(STATUS "Git_FOUND: ${Git_FOUND}")
MESSAGE(STATUS "Boost_FOUND: ${Boost_FOUND}")
MESSAGE(STATUS "OpenSSL_FOUND: ${OpenSSL_FOUND}")
MESSAGE(STATUS "ZLIB_FOUND: ${ZLIB_FOUND}")
MESSAGE(STATUS "Protobuf_FOUND: ${Protobuf_FOUND}")

IF(Boost_FOUND)
    SET(BOOST_INCLUDE ${BOOST_INCLUDEDIR})
ENDIF()
ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
include_directories(${BOOST_INCLUDE})

IF(WIN32)
    IF(CMAKE_CL_64)
        set(PLATDIR x64)
    ELSE()
        set(PLATDIR Win32)
    ENDIF()
ENDIF()

# for vs folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


include(./cmake/config.cmake)
include(./cmake/func.cmake)

IF(WIN32)
    ADD_DEFINITIONS(-D_WIN32_WINNT=0x0603)
    ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
    ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINITIONS(-D_WINSOCK_DEPRECATED_NO_WARNINGS)
ELSE()
ENDIF()

message(STATUS "CMAKE_CXX_FLAGS_DEBUG " ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "CMAKE_C_FLAGS_DEBUG " ${CMAKE_C_FLAGS_DEBUG})

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "Release")
ENDIF()
message(STATUS "CMAKE_BUILD_TYPE " ${CMAKE_BUILD_TYPE})
IF(${CMAKE_BUILD_TYPE} MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG)
ENDIF()

IF(NOT CMAKE_PREFIX_PATH)
    IF(CMAKE_CL_64)
        set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/target/x64)
    ELSE()
        set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/target)
    ENDIF()
ENDIF()
message(STATUS "CMAKE_PREFIX_PATH " ${CMAKE_PREFIX_PATH})

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

link_directories(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})


# SET CXX FLAGS
IF(WIN32)
    add_compile_options(/wd4068 /wd4251 /wd4275 /wd4819 /wd4244 /W3 /nologo /EHsc)
ELSE()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg -ggdb3 -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

    #set(CMAKE_C_FLAGS "-pthread -I./ -msse4.1")
    set(CMAKE_C_FLAGS "-pthread -march=native -O3 -maes -msse2 -mrdseed")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

    set(CMAKE_CXX_FLAGS "-pthread -march=native -O3 -maes -msse2 -mrdseed")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpthread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    
    # below warnings will be solved later
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-reorder")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-but-set-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-function")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-result")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-value")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-sign-compare")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
ENDIF()


IF(WIN32)
    SET(OT_NP_USE_MIRACL ON)
    ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)
ENDIF()


# encrypto libraries
IF(OT_NP_USE_MIRACL)
    ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)
    add_subdirectory(third-code/miracl)
ELSEIF(WIN32 AND NOT OT_NP_USE_MIRACL)
    add_subdirectory(third-code/relic-win)
ELSEIF(UNIX AND NOT OT_NP_USE_MIRACL)
    add_subdirectory(third-code/relic)
ENDIF()


## add emp-toolkit submodules
add_subdirectory(third-code/emp-tool)
add_subdirectory(third-code/emp-ot)
add_subdirectory(third-code/emp-sh2pc)

# 
# third-code projects
# 
add_subdirectory(third-code/gflags)

SET(gtest_force_shared_crt ON)
add_subdirectory(third-code/gtest)

option(protobuf_BUILD_TESTS "Build tests" OFF)
option(protobuf_BUILD_SHARED_LIBS "Build tests" ON)
add_subdirectory(third-code/protobuf/cmake)


IF(ENABLE_ENCRYPT)
    add_subdirectory(third-code/secp256k1)

    # compile libscrypt beg
    file(GLOB libscrypt_SOURCES "${THIRD_CODE_DIR}/libscrypt/*.c")
    file(GLOB libscrypt_HEADERS "${THIRD_CODE_DIR}/libscrypt/*.h")
   
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
    add_library(libscrypt_static STATIC ${libscrypt_SOURCES} ${libscrypt_HEADERS})
    SET_PROPERTY(TARGET libscrypt_static PROPERTY FOLDER third-code/scrypt)
        # compile libscrypt end
ENDIF()
add_subdirectory(third-code/eth-simplest)



# platon mpc vm projects
add_subdirectory(src/mpc-vm-common)
add_subdirectory(src/mpc-vm-service)
add_subdirectory(src/mpc-vm-sdk)
add_subdirectory(src/mpc-vm-platon-sdk)


# 
# tools
# 
add_subdirectory(tools/gencode)


# 
# test projects
# 
#add_subdirectory(test/juzix-mpc)
#add_subdirectory(test/juzix-mpc-sdk)
add_subdirectory(test/mpc-vm-common)
#add_subdirectory(test/mpc-vm-sdk)
#add_subdirectory(test/mpc-vm-core)
add_subdirectory(test/mpc-vm-service)
#add_subdirectory(test/mpc-vm-platon-sdk)
#add_subdirectory(test/mpc-vm-app)

