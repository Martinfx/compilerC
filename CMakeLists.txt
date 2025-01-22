cmake_minimum_required(VERSION 3.16)

project(compilerC LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(compilerC main.cpp
    compiler.hpp
    lexer.hpp)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

enable_testing()
add_test(NAME runTests COMMAND runTests)

include(GNUInstallDirs)
install(TARGETS compilerC
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
