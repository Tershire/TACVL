cmake_minimum_required(VERSION 3.2)
project(Basic)

find_package(OpenCV 4 REQUIRED)

add_executable(show_image show_image.cpp)
target_link_libraries(show_image ${OpenCV_LIBS})