cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(color_picker_BGR_to_HSV color_picker_BGR_to_HSV.cpp)
add_executable(color_picker_HSV_to_BGR color_picker_HSV_to_BGR.cpp)
add_executable(read_numerics_from_txt read_numerics_from_txt.cpp)

target_link_libraries(color_picker_BGR_to_HSV ${OpenCV_LIBS})
target_link_libraries(color_picker_HSV_to_BGR ${OpenCV_LIBS})