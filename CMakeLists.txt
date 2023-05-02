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

#[[
FILE(GLOB ILL_TEST_FILES "tests/*/test*")
foreach(FILE_PATH IN LISTS ILL_TEST_FILES)
    add_test(NAME ${FILE_PATH} COMMAND stella-interpreter typecheck ${FILE_PATH})
endforeach()

set_tests_properties(${ILL_TEST_FILES} PROPERTIES WILL_FAIL TRUE)

FILE(GLOB WELL_TEST_FILES "tests/*/well-typed/*")
foreach(FILE_PATH IN LISTS WELL_TEST_FILES)
    add_test(NAME ${FILE_PATH} COMMAND stella-interpreter typecheck ${FILE_PATH})
endforeach()]]
