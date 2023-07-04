cmake_minimum_required(VERSION 3.2)
project(Basic)

find_package(OpenCV 4 REQUIRED)

add_executable(show_image show_image.cpp)
add_executable(stream_video stream_video.cpp)
add_executable(read_image_sequence read_image_sequence.cpp)

target_link_libraries(show_image ${OpenCV_LIBS})
target_link_libraries(stream_video ${OpenCV_LIBS})
target_link_libraries(read_image_sequence ${OpenCV_LIBS})