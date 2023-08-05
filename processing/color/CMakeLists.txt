cmake_minimum_required(VERSION 3.2)
project(processing/color)

find_package(OpenCV 4 REQUIRED)

add_executable(color_detection_stream color_detection_stream.cpp)

target_link_libraries(color_detection_stream ${OpenCV_LIBS})
