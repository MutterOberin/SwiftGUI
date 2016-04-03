################################################################
# Project SwiftGUI and examples
################################################################

project(SwiftGUI CXX)
cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(CEF REQUIRED)

find_package(GLUT)
find_package(OpenGL)
find_package(GLEW)

set(CMAKE_INSTALL_RPATH
    "${CMAKE_INSTALL_PREFIX}/lib"
    "${CEF_LIBRARY_DIR}"
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -Ofast --std=c++11")

add_subdirectory(src)
add_subdirectory(examples)
