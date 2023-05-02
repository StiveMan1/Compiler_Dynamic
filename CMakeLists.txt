cmake_minimum_required(VERSION 3.17.3)
project(Compiler_Dynamic C)

set(CMAKE_C_STANDARD 99)

FILE(GLOB SOURCE_FILES_DIR_DIR "src/*/*/*.c")
FILE(GLOB SOURCE_FILES_DIR "src/*/*.c")
FILE(GLOB SOURCE_FILES "src/*.c")
add_library(${PROJECT_NAME}_lib ${SOURCE_FILES} ${SOURCE_FILES_DIR} ${SOURCE_FILES_DIR_DIR})
target_include_directories(${PROJECT_NAME}_lib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)
MESSAGE(STATUS "Connected: ${PROJECT_NAME}_lib")

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJECT_NAME}_lib)
add_executable(test test.c)
target_link_libraries(test PUBLIC ${PROJECT_NAME}_lib)

FILE(GLOB TEST_FILES "tests/*/text*/*.txt")
foreach(TEST_PATH IN LISTS TEST_FILES)
    add_test(NAME ${TEST_PATH} COMMAND test ${TEST_PATH})
endforeach()