#cmake_minimum_required (VERSION 2.8.11)
#project (miracl)

set(NAME "miracl")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB miracl_headers include/*.h)
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/source)
aux_source_directory(${CUR_DIR}/source-used SRC_LIST)


IF(WIN32)
ELSE()
    set(CMAKE_C_FLAGS "-fPIC ${CMAKE_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
ENDIF()


#
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#

#generate static lib
add_library(${NAME}_static STATIC ${SRC_LIST} ${miracl_headers})
SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "third-code"
    )

INSTALL(TARGETS ${NAME}_static
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )
INSTALL(FILES ${miracl_headers} DESTINATION ${CMAKE_PREFIX_PATH}/include/miracl)


# 
# 
# 
# generate project miracl-examples
FUNCTION(gen_proj_me projname) # projname srcs...
    set(srcs "")
    foreach(src IN ITEMS ${ARGN})
        list(APPEND srcs ${CUR_DIR}/source/${src})
    endforeach()
    add_executable(${projname} ${srcs})
    target_link_libraries(${projname} ${NAME}_static)
    SET_TARGET_PROPERTIES(${projname} PROPERTIES FOLDER "miracl-examples")
    INSTALL(TARGETS ${projname} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
ENDFUNCTION()

#gen_proj_me(bmark       bmark.c)
#gen_proj_me(fact        fact.c)
#gen_proj_me(mersenne    mersenne.cpp big.cpp)
#gen_proj_me(brent       brent.cpp big.cpp zzn.cpp )
#gen_proj_me(sample      sample.cpp flash.cpp)

#gen_proj_me(ecsgen      ecsgen.cpp ecn.cpp big.cpp)
#gen_proj_me(ecsign      ecsign.cpp ecn.cpp big.cpp)
#gen_proj_me(ecsver      ecsver.cpp ecn.cpp big.cpp)
#gen_proj_me(pk-demo     pk-demo.cpp ecn.cpp big.cpp)

#gen_proj_me(curve-schoof crt.cpp zzn.cpp ecn.cpp big.cpp curve/schoof.cpp curve/polymod.cpp curve/poly.cpp)







