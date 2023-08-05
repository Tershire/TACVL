cmake_minimum_required(VERSION 3.2)
project(structure_from_motion/triangulation)

find_package(OpenCV 4 REQUIRED)

add_executable(triangulate_kannala_brandt triangulate_kannala_brandt.cpp)

target_link_libraries(triangulate_kannala_brandt ${OpenCV_LIBS})
