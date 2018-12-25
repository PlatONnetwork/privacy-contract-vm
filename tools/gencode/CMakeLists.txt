CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(gencode)

SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 11)

set(NAME libgencode)

set(SRCS 
src/gencode.cpp
src/gencode.h
)

add_library(${NAME} SHARED ${SRCS})
IF(WIN32)
target_compile_options(${NAME} PRIVATE /D "GENCODE_EXPORTS")
ENDIF()
SET_PROPERTY(TARGET ${NAME} PROPERTY FOLDER tools/gencode)
INSTALL(TARGETS ${NAME} 
	RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/target/bin
	LIBRARY DESTINATION ${CMAKE_SOURCE_DIR}/target/lib
)

add_library(${NAME}_static STATIC ${SRCS})
IF(WIN32)
target_compile_options(${NAME}_static PRIVATE /D "GENCODE_STATIC")
ENDIF()
SET_PROPERTY(TARGET ${NAME}_static PROPERTY FOLDER tools/gencode)
INSTALL(TARGETS ${NAME}_static 
	ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/target/lib
)

set(NAME gencode)
set(SRCS 
src/gencode_main.cpp
src/gencode.h
)

ADD_EXECUTABLE(${NAME}_main ${SRCS})
target_link_libraries(${NAME}_main libgencode)
SET_PROPERTY(TARGET ${NAME}_main PROPERTY FOLDER tools/gencode)
INSTALL(TARGETS ${NAME}_main RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/target/bin)
