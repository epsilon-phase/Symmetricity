cmake_minimum_required(VERSION 2.8)

project(symmetricitity)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Wall")

add_executable(symmetricitity main.cpp ReflectionMatrix.cpp agony.cpp)
target_include_directories(symmetricitity PUBLIC /usr/include/eigen3)
set(CMAKE_MODULE_PATH "./cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_BUILD_TYPE Debug)
find_package(SFML 2.2.0 REQUIRED system window graphics network audio)
target_link_libraries(symmetricitity ${SFML_LIBRARIES})