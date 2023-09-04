cmake_minimum_required(VERSION 3.2)
project(processing/connected_components)

find_package(OpenCV 4 REQUIRED)

add_executable(connected_components connected_components.cpp)

target_link_libraries(connected_components ${OpenCV_LIBS})
