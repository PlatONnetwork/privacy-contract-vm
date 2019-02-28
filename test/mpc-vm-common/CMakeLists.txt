#cmake_minimum_required (VERSION 2.8.11)
project (mpc_vm_common_test)

set(NAME "mpc_vm_common_test")
set(CUR_DIR ${CMAKE_CURRENT_SOURCE_DIR})

GenOrganizedTree("" ${CUR_DIR} *.*)


include_directories(${THIRD_CODE_DIR}/gtest/googletest/include)

# openssl for relic
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
link_libraries(${OPENSSL_LIBRARIES})

set(MPC_VM_COMMON_DIR ${CMAKE_SOURCE_DIR}/src/mpc-vm-common)
include_directories(${MPC_VM_COMMON_DIR}/include)
include_directories(${MPC_VM_COMMON_DIR}/utils)

FUNCTION(compile_test projname)
	if (${CMAKE_BUILD_TYPE} MATCHES Debug)
		link_libraries(gtestd gtest_maind gmockd gmock_maind)
	else()	
		link_libraries(gtest gtest_main gmock gmock_main)
	endif()

	add_executable(mpc-vm-common-utest-${projname} unittest/${projname}.cpp)
	target_link_libraries(mpc-vm-common-utest-${projname} mpc_vm_common_static)

	SET_TARGET_PROPERTIES(mpc-vm-common-utest-${projname} PROPERTIES  FOLDER "mpc_vm_test/mpc-vm-common/unittest")
	install_libraries(mpc-vm-common-utest-${projname})
ENDFUNCTION()

compile_test(base64)
compile_test(rsa)
compile_test(ssl)
compile_test(utils2)
compile_test(safequeue)
