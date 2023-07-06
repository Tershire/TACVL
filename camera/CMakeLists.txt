cmake_minimum_required(VERSION 3.2)
project(Camera)

find_package(OpenCV 4 REQUIRED)

add_executable(undistort undistort.cpp)

target_link_libraries(undistort ${OpenCV_LIBS})