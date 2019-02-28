CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(gencode)
set(NAME libgencode)

set(SRCS 
    src/gencode.cpp
    src/gencode.h
)

# dynamic
add_library(${NAME} SHARED ${SRCS})
IF(WIN32)
    target_compile_options(${NAME} PRIVATE /D "GENCODE_EXPORTS")
ENDIF()
SET_PROPERTY(TARGET ${NAME} PROPERTY FOLDER tools/gencode)

# static
add_library(${NAME}_static STATIC ${SRCS})
IF(WIN32)
    target_compile_options(${NAME}_static PRIVATE /D "GENCODE_STATIC")
ELSE()
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
ENDIF()
SET_PROPERTY(TARGET ${NAME}_static PROPERTY FOLDER tools/gencode)
install_libraries(${NAME} ${NAME}_static)

# main for test
set(NAME gencode)
    set(SRCS 
    src/gencode_main.cpp
    src/gencode.h
)

ADD_EXECUTABLE(${NAME}_main ${SRCS})
target_link_libraries(${NAME}_main libgencode)
SET_PROPERTY(TARGET ${NAME}_main PROPERTY FOLDER tools/gencode)
install_libraries(${NAME}_main)
