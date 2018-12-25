cmake_minimum_required(VERSION 3.3)
project(eth-simplest)

IF(NOT WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

ADD_DEFINITIONS(-DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
include_directories(${BOOST_INCLUDE})

IF(WIN32)
	include_directories(${WIN_THIRD_COMPILED_DIR}/jsoncpp/include)
	link_directories(${WIN_THIRD_COMPILED_DIR}/jsoncpp/lib/${PLATDIR})
		
	link_libraries(jsoncpp)
    
	include_directories(${THIRD_CODE_DIR}/secp256k1/include)
	include_directories(${THIRD_CODE_DIR}/libscrypt)
	
ELSE()
    find_package(Jsoncpp REQUIRED)
    
	link_directories(/usr/local/lib /usr/lib /usr/lib64)
	link_libraries(jsoncpp)
	
	include_directories(${THIRD_CODE_DIR}/secp256k1/include)
	include_directories(${THIRD_CODE_DIR}/libscrypt)
ENDIF()

IF(ENABLE_ENCRYPT)
    IF(WIN32)
	    include_directories(${WIN_THIRD_COMPILED_DIR}/cryptopp/include)
	    link_directories(${WIN_THIRD_COMPILED_DIR}/cryptopp/lib/${PLATDIR})
	    link_libraries(cryptlib)
    ELSE()
	    link_libraries(cryptopp)
    ENDIF()
ENDIF()

include_directories(.)

# libdevcore
SET(LIBDEVCORE_SIMPLEST libdevcore-simplest)
file(GLOB libdevcore_sources "libdevcore/*.cpp")
file(GLOB libdevcore_headers "libdevcore/*.h")
add_library(${LIBDEVCORE_SIMPLEST} STATIC ${libdevcore_sources} ${libdevcore_headers})
SET_PROPERTY(TARGET ${LIBDEVCORE_SIMPLEST} PROPERTY FOLDER third-code/eth-simplest)

# libdevcrypto
SET(LIBDEVCRYPTO_SIMPLEST libdevcrypto-simplest)
file(GLOB libdevcrypto_sources "libdevcrypto/*.cpp")
file(GLOB libdevcrypto_headers "libdevcrypto/*.h")
add_library(${LIBDEVCRYPTO_SIMPLEST} STATIC ${libdevcrypto_sources} ${libdevcrypto_headers})
target_link_libraries(${LIBDEVCRYPTO_SIMPLEST} ${LIBDEVCORE_SIMPLEST} secp256k1 libscrypt_static)
SET_PROPERTY(TARGET ${LIBDEVCRYPTO_SIMPLEST} PROPERTY FOLDER third-code/eth-simplest)

# demo
SET(ETH_DEMO eth-simplest-demo1)
add_executable(${ETH_DEMO} examples/demo1.cpp)
target_link_libraries(${ETH_DEMO} ${LIBDEVCORE_SIMPLEST})
SET_PROPERTY(TARGET ${ETH_DEMO} PROPERTY FOLDER third-code/eth-simplest/examples)

