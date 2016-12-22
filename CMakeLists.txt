project(fluentcontainer)
cmake_minimum_required(VERSION 3.1.0)

# Options
option(ENABLE_TESTS "Enable tests and coverage" OFF)

# Configuration
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_STANDARD 14)

if(ENABLE_TESTS)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        message("Building with tests and coverage")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")
    endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif(ENABLE_TESTS)


# Projects
add_subdirectory(src/3rdparty)
if(ENABLE_TESTS)
    add_subdirectory(src/tests)
endif(ENABLE_TESTS)


