cmake_minimum_required(VERSION 3.30)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic -pedantic-errors -Wconversion)
include_directories(include)
add_executable(NetSim
    main.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        src/package.cpp
        )

add_executable(NetSim_test
        src/storage_types.cpp
        test/main_test.cpp
        test/tests.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        src/package.cpp
)

set(GTEST_ROOT ext/googletest-master)
target_include_directories(NetSim_test PRIVATE
        ${GTEST_ROOT}/googlemock/include
        ${GTEST_ROOT}/googletest/include
)

add_subdirectory(${GTEST_ROOT} googletest-master)

target_link_libraries(NetSim_test
        PRIVATE
        gtest_main
        gtest
        mingw32
        mingwex
        ucrt
)
