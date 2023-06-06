cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(track track.cpp)

target_link_libraries(track ${OpenCV_LIBS})