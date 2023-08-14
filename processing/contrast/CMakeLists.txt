cmake_minimum_required(VERSION 3.2)
project(processing/contrast)

find_package(OpenCV 4 REQUIRED)

add_executable(clahe_stream clahe_stream.cpp)

target_link_libraries(clahe_stream ${OpenCV_LIBS})
