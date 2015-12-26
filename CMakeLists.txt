cmake_minimum_required(VERSION 2.8)

project(symmetricity)
find_package(OpenMP)
find_package(eigen3)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS} -std=c++1y -Wall")
#else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
#endif()


add_executable(symmetricity main.cpp TransformationMatrix.cpp agony.cpp pathfinder.cpp designation_menu.cpp city.cc)

set(CMAKE_MODULE_PATH "./cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_BUILD_TYPE Debug)
find_package(SFML 2.2.0 REQUIRED system window graphics network audio)
target_link_libraries(symmetricity ${SFML_LIBRARIES})