cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(canny_edge_image canny_edge_image.cpp)
add_executable(canny_edge_stream canny_edge_stream.cpp)
add_executable(blob_stream blob_stream.cpp)


target_link_libraries(canny_edge_image ${OpenCV_LIBS})
target_link_libraries(canny_edge_stream ${OpenCV_LIBS})
target_link_libraries(blob_stream ${OpenCV_LIBS})