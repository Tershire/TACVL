cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(color_picker_BGR_trackbar color_picker_BGR_trackbar.cpp)

target_link_libraries(color_picker_BGR_trackbar ${OpenCV_LIBS})