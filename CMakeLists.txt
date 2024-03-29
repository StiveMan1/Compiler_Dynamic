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
add_executable(dynamic_test dynamic_test.c)
target_link_libraries(dynamic_test PUBLIC ${PROJECT_NAME}_lib)

ENABLE_TESTING()

INCLUDE(CTest)

FILE(GLOB TEST_FILES "tests/*/*")
foreach(TEST_PATH IN LISTS TEST_FILES)
    add_test(NAME ${TEST_PATH} COMMAND dynamic_test ${TEST_PATH})
endforeach()

set_tests_properties(${TEST_FILES} PROPERTIES WILL_FAIL FALSE)