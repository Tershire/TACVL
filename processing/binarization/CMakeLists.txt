cmake_minimum_required(VERSION 3.2)
project(processing/binarization)

find_package(OpenCV 4 REQUIRED)

add_executable(otsu_stream otsu_stream.cpp)
add_executable(otsu_image otsu_image.cpp)

target_link_libraries(otsu_stream ${OpenCV_LIBS})
target_link_libraries(otsu_image ${OpenCV_LIBS})
