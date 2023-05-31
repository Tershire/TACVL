cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(otsu_canny_stream otsu_canny_stream.cpp)

target_link_libraries(otsu_canny_stream ${OpenCV_LIBS})