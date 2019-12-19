cmake_minimum_required(VERSION 3.15)

project(IgnisCor C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_INSTALL_PREFIX ./build)
set(BUILD_SHARED_LIBS Off)
set(SFML_USE_STATIC_STD_LIBS On)

include_directories(lib/SFML/include)
add_definitions(-DSFML_STATIC)

add_subdirectory(lib/SFML)
add_subdirectory(src)