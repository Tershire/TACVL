cmake_minimum_required(VERSION 3.2)
project(processing/mask)

find_package(OpenCV 4 REQUIRED)

add_executable(circular_mask circular_mask.cpp)

target_link_libraries(circular_mask ${OpenCV_LIBS})
