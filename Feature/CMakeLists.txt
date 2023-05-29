cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(canny_test_image canny_test_image.cpp)
target_link_libraries(canny_test_image ${OpenCV_LIBS})