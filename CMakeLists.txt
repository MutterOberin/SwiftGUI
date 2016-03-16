################################################################
# Project SwiftGUI and examples
################################################################

project(SwiftGUI CXX)
cmake_minimum_required(VERSION 2.8.0)

set(CEF_DIR "$ENV{CEF_DIR}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -Ofast --std=c++11")

add_subdirectory(src)
add_subdirectory(examples)
