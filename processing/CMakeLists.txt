cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(otsu_stream otsu_stream.cpp)

target_link_libraries(otsu_stream ${OpenCV_LIBS})