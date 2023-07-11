cmake_minimum_required(VERSION 3.2)
project(Camera)

find_package(OpenCV 4 REQUIRED)

add_executable(undistort undistort.cpp)
add_executable(kannala_brandt_unproject kannala_brandt_unproject.cpp)

target_link_libraries(undistort ${OpenCV_LIBS})
target_link_libraries(kannala_brandt_unproject ${OpenCV_LIBS})