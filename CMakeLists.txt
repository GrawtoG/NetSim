cmake_minimum_required(VERSION 3.30)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -Wpedantic -pedantic-errors -o2")
add_executable(NetSim
    main.cpp
        src/storage_types.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
        )

add_executable(NetSim_test
        main_test.cpp
        src/storage_types.cpp
        test/tests.cpp
        src/nodes.cpp
        src/helpers.cpp
        src/factory.cpp
)
target_include_directories(NetSim_test PUBLIC
        ext/googletest-master/googlemock/include
        ext/googletest-master/googletest/include
)