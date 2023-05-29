cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(canny_edge_image canny_edge_image.cpp)
target_link_libraries(canny_edge_image ${OpenCV_LIBS})