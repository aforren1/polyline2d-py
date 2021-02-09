cmake_minimum_required (VERSION 3.0)
set(CMAKE_CXX_STANDARD 17)
project(polyline2d_py)

set(DEPS_DIR "${CMAKE_CURRENT_LIST_DIR}/deps")
include_directories("${DEPS_DIR}/polyline2d/include")
add_subdirectory("${DEPS_DIR}/pybind11")
pybind11_add_module(polyline2d_py src/main.cpp)

set_target_properties(polyline2d_py PROPERTIES OUTPUT_NAME "polyline2d")
