cmake_minimum_required(VERSION 3.2)
project(processing/contour/tutorial)

find_package(OpenCV 4 REQUIRED)

add_executable(tutorial_contour tutorial_contour.cpp)

target_link_libraries(tutorial_contour ${OpenCV_LIBS})
