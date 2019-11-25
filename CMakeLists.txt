#------------------------------------------------------------------------------
# CMake file for Bigbang
#
# Copyright (c) 2019 The Bigbang developers
# Distributed under the MIT/X11 software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5.1)
project(OffLineSignLib LANGUAGES C CXX)

set(CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(sodium 1.0.8 REQUIRED)


set(sources
    src/signlib.cpp
    src/signlib.h
    src/uint256.h
    src/uint256.cpp
    src/util.h
)

ADD_LIBRARY (offlinesign SHARED ${sources})

include_directories(./ ./src ./jni ${sodium_INCLUDE_DIR})
target_link_libraries(offlinesign
    ${sodium_LIBRARY_RELEASE}
)

set(exec_src test/main_test.cpp)
add_executable(test_lib ${exec_src})

target_link_libraries(test_lib offlinesign ${sodium_LIBRARY_RELEASE})

install(TARGETS offlinesign
        LIBRARY DESTINATION lib)