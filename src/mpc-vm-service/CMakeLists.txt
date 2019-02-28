cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_service)

set(NAME "mpc_vm_service")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
GenOrganizedTree("" ${CUR_DIR} *.*)


###### includes here
file(GLOB_RECURSE mpc_vm_sdk_headers ${CUR_DIR}/*.h ${CUR_DIR}/*.ice ${CUR_DIR}/*.proto ${CUR_DIR}/*.json)

# includes
include_directories(${THIRD_CODE_DIR}/protobuf/src)
include_directories(${CUR_DIR}/include)
include_directories(${CUR_DIR}/io)
include_directories(${CUR_DIR}/core)
include_directories(${CUR_DIR}/task)
include_directories(${CUR_DIR}/server)
include_directories(${CUR_DIR}/client)
include_directories(${CUR_DIR}/callback)
include_directories(${CUR_DIR}/jrpc)
include_directories(${CUR_DIR}/slice)

include_directories(${CUR_DIR}/test)
include_directories(${CUR_DIR}/testimpl)

include_directories(${CUR_DIR}/../mpc-vm-common/include)
include_directories(${CUR_DIR}/../mpc-vm-common/utils)

include_directories(${THIRD_CODE_DIR}/emp-tool)
include_directories(${THIRD_CODE_DIR}/emp-tool/include)
include_directories(${THIRD_CODE_DIR}/emp-ot)
include_directories(${THIRD_CODE_DIR}/emp-sh2pc)

include_directories(${THIRD_CODE_DIR}/jit/include)
include_directories(${THIRD_CODE_DIR}/eth-simplest)

include_directories(${ICE_ROOT}/include)

#source list
aux_source_directory(. SRC_LIST)
aux_source_directory(./task SRC_LIST)
aux_source_directory(./core SRC_LIST)
aux_source_directory(./jrpc SRC_LIST)
aux_source_directory(./client SRC_LIST)
aux_source_directory(./server SRC_LIST)
aux_source_directory(./callback SRC_LIST)
aux_source_directory(./io SRC_LIST)
aux_source_directory(./slice SRC_LIST)


###### libraries here
IF(WIN32)
    link_libraries(Ws2_32)
ENDIF()

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
link_libraries(${OPENSSL_LIBRARIES})

link_libraries(mpc_vm_common_static)
link_libraries(emp-tool)

link_ice() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_mpc_jit() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
support_jsonrpc() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_relic_or_miracl() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake

link_libraries(libdevcore-simplest)
IF(ENABLE_ENCRYPT)
    IF(WIN32)
	    link_directories(${WIN_THIRD_COMPILED_DIR}/cryptopp/lib/${PLATDIR})
    ELSE()
	    link_directories(${LINUX_THIRD_COMPILED_DIR}/cryptopp/lib)
    ENDIF()
    link_libraries(libdevcrypto-simplest)
ENDIF()


###### build targets here
# dynamic
add_library(${NAME} SHARED ${SRC_LIST} ${mpc_vm_sdk_headers})

IF(WIN32)
	target_compile_options(${NAME} PRIVATE /D "PLATONMPCVM_SERVICE_EXPORTS")
ENDIF()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
	FOLDER "mpc_vm"
	VERSION ${SO_VERSION}
)

IF(WIN32)
    IF(CMAKE_CL_64)
        set(lib_plat "x64/")
    ELSE()
        set(lib_plat "")
    ENDIF()
	add_custom_command(TARGET ${NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${WIN_THIRD_COMPILED_DIR}/curl/lib/${PLATDIR}/$<CONFIGURATION>/libcurl.dll"
			$<TARGET_FILE_DIR:${NAME}>)
	INSTALL(FILES "${WIN_THIRD_COMPILED_DIR}/curl/lib/${PLATDIR}/$<CONFIGURATION>/libcurl.dll" DESTINATION ${CMAKE_PREFIX_PATH}/bin)

    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${THIRD_CODE_DIR}/jit/lib/win/${PLATDIR}/$<CONFIGURATION>/mpc-jit.dll"
		$<TARGET_FILE_DIR:${NAME}>)
        
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/bzip2d.dll" $<TARGET_FILE_DIR:${NAME}>)
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/glacier236d.dll" $<TARGET_FILE_DIR:${NAME}>)
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/ice36d.dll" $<TARGET_FILE_DIR:${NAME}>)
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/iceutil36d.dll" $<TARGET_FILE_DIR:${NAME}>)

    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/bzip2.dll" $<TARGET_FILE_DIR:${NAME}>)
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/glacier236.dll" $<TARGET_FILE_DIR:${NAME}>)
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/ice36.dll" $<TARGET_FILE_DIR:${NAME}>)
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${ICE_ROOT}/bin/${lib_plat}/iceutil36.dll" $<TARGET_FILE_DIR:${NAME}>)

ELSE()
    add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${THIRD_CODE_DIR}/jit/lib/libmpc-jit.so"
		$<TARGET_FILE_DIR:${NAME}>)
	INSTALL(FILES "${THIRD_CODE_DIR}/jit/lib/libmpc-jit.so" DESTINATION ${CMAKE_PREFIX_PATH}/lib)
	INSTALL(FILES "${THIRD_CODE_DIR}/jit/lib/libmpc-jit.so.6" DESTINATION ${CMAKE_PREFIX_PATH}/lib)
	INSTALL(FILES "${THIRD_CODE_DIR}/jit/lib/libmpc-jit.so.6.0.1" DESTINATION ${CMAKE_PREFIX_PATH}/lib)
   
ENDIF()
add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"${CMAKE_SOURCE_DIR}/config/mpcc.cpp.abi.json"
		$<TARGET_FILE_DIR:${NAME}>)


# static
add_library(${NAME}_static STATIC ${SRC_LIST} ${mpc_vm_sdk_headers})

IF(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "PLATONMPCVM_SERVICE_STATIC")
ELSE()
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
ENDIF()

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
	FOLDER "mpc_vm"
	VERSION ${SO_VERSION}
)

install_libraries(${NAME} ${NAME}_static)
