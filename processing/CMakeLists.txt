cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(canny_image canny_image.cpp)
add_executable(canny_stream canny_stream.cpp)
add_executable(otsu_stream otsu_stream.cpp)

target_link_libraries(canny_image ${OpenCV_LIBS})
target_link_libraries(canny_stream ${OpenCV_LIBS})
target_link_libraries(otsu_stream ${OpenCV_LIBS})