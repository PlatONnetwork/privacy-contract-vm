#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_sdk)

set(NAME "mpc_vm_sdk")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)

# find_package(OpenSSL)
#include_directories(${OPENSSL_INCLUDE_DIR})

# include current directory *.h
file(GLOB_RECURSE juzixmpc_headers ${CUR_DIR}/*.h)

# includes
include_directories(${ICE_ROOT}/include)
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/../mpc-vm-common/include)
include_directories(${CUR_DIR}/../mpc-vm-service/slice)
include_directories(${CUR_DIR}/api)
include_directories(${CUR_DIR}/test)
include_directories(${CUR_DIR}/cb)
include_directories(${CUR_DIR}/client)


#source list
aux_source_directory(.          SRC_LIST)
aux_source_directory(./include  SRC_LIST)
aux_source_directory(./api		SRC_LIST)
aux_source_directory(./cb		SRC_LIST)
aux_source_directory(./test		SRC_LIST)
aux_source_directory(./client	SRC_LIST)

aux_source_directory(../mpc-vm-service/slice		SRC_LIST)


## print directory #####
# message("the include directory: ${INCLUDE_DIRECTORIES}" )
# message("the src dir: ${SRC_LIST}")


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

set(lib_plat "")
IF(CMAKE_CL_64)
    set(lib_plat "x64/")
ENDIF()



# link ice
IF(WIN32)
    link_directories(${ICE_ROOT}/lib/${lib_plat})
ELSE()
    link_directories(
    /usr/lib64/c++11
    /usr/lib/x86_64-linux-gnu/c++11
    ${ICE_ROOT}/lib/x86_64-linux-gnu/c++11
    ${ICE_ROOT}/lib64
    ${ICE_ROOT}/lib64/c++11
    )
ENDIF()

link_directories(${CMAKE_BINARY_DIR}/lib ${ICE_ROOT}/lib)

IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

#link_libraries(mpc_vm_service)

# dynamic
add_library(${NAME} SHARED ${SRC_LIST} ${juzixmpc_headers})
IF(WIN32)
target_compile_options(${NAME} PRIVATE /D "PLATONMPCVM_SDK_EXPORTS")
ENDIF()


function(LinkIce projname)
    IF(WIN32)
        target_link_libraries(${projname} debug Iced)
        target_link_libraries(${projname} debug Glacier2d)
        target_link_libraries(${projname} debug IceUtild)
        target_link_libraries(${projname} optimized Ice)
        target_link_libraries(${projname} optimized Glacier2)
        target_link_libraries(${projname} optimized IceUtil)
    ELSE()
        target_link_libraries(${projname} Ice Glacier2 IceUtil)
    ENDIF()
endfunction()
LinkIce(${NAME})

SET_TARGET_PROPERTIES(${NAME} PROPERTIES 
	FOLDER "mpc_vm"
)

# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${juzixmpc_headers})
IF(WIN32)
target_compile_options(${NAME}_static PRIVATE /D "PLATONMPCVM_SDK_STATIC")
ENDIF()
LinkIce(${NAME}_static)

IF(WIN32)
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES FOLDER "mpc_vm")
ELSE()
SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "mpc_vm"
    )
ENDIF()


INSTALL(TARGETS ${NAME} ${NAME}_static
    RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin
    LIBRARY DESTINATION ${CMAKE_PREFIX_PATH}/lib
    ARCHIVE DESTINATION ${CMAKE_PREFIX_PATH}/lib
    )

file(GLOB_RECURSE  include_headers      ${CUR_DIR}/include/*.h)
INSTALL(FILES ${include_headers}        DESTINATION ${CMAKE_PREFIX_PATH}/include/vmsdk)

#install(FILES ${headers} DESTINATION include/${NAME})
#install(TARGETS ${NAME} DESTINATION lib)


# 
# 
# 
# EXAMPLES EXAMPLES EXAMPLES EXAMPLES
FUNCTION(CopyIceDLLCMD projname libname)
    add_custom_command(TARGET ${projname} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ICE_ROOT}/bin/${lib_plat}${libname}.dll"
        $<TARGET_FILE_DIR:${projname}>)
    add_custom_command(TARGET ${projname} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ICE_ROOT}/bin/${lib_plat}${libname}d.dll"
        $<TARGET_FILE_DIR:${projname}>)
ENDFUNCTION()
FUNCTION(InstallIceDLLCMD libname)
    INSTALL(FILES "${ICE_ROOT}/bin/${lib_plat}${libname}.dll" DESTINATION ${CMAKE_PREFIX_PATH}/bin)
    INSTALL(FILES "${ICE_ROOT}/bin/${lib_plat}${libname}d.dll" DESTINATION ${CMAKE_PREFIX_PATH}/bin)
ENDFUNCTION()

macro(compile_test _name)
	set(projname ${NAME}-${_name})
    aux_source_directory(./main EXE_SRC_LIST)
	
    add_executable(${projname} ${EXE_SRC_LIST})
    
    target_link_libraries(${projname} ${NAME})
	SET_TARGET_PROPERTIES(${projname} PROPERTIES FOLDER "mpc_vm")
    INSTALL(TARGETS ${projname} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)

    IF(WIN32)
		CopyIceDLLCMD(${projname} ice36)
		CopyIceDLLCMD(${projname} glacier236)
		CopyIceDLLCMD(${projname} iceutil36)
		CopyIceDLLCMD(${projname} bzip2)
		InstallIceDLLCMD(ice36)
		InstallIceDLLCMD(glacier236)
		InstallIceDLLCMD(iceutil36)
		InstallIceDLLCMD(bzip2)
    ENDIF()

endmacro()


compile_test(client)



