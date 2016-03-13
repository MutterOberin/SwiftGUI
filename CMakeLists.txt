################################################################
# Project SwiftGUI and examples
################################################################

project(SwiftGUI CXX)
cmake_minimum_required(VERSION 2.8.0)

set(CEF_INCLUDE_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/../../cef"
)

set(CEF_BINARY_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/../../cef/Release"
)

set(CEF_RESOURCE_DIR
  "${CMAKE_CURRENT_SOURCE_DIR}/../../cef/Resources"
)

set(CEF_LIBRARY_DIRS
  "${CMAKE_CURRENT_SOURCE_DIR}/../../cef/Release"
  "${CMAKE_CURRENT_SOURCE_DIR}/../../cef/build/libcef_dll"
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -Ofast --std=c++0x")

add_subdirectory(src)
add_subdirectory(examples)
