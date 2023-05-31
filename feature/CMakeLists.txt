cmake_minimum_required(VERSION 3.2)
project(Feature)

find_package(OpenCV 4 REQUIRED)

add_executable(canny_image canny_image.cpp)
add_executable(canny_stream canny_stream.cpp)
add_executable(blob_stream blob_stream.cpp)
add_executable(orb_1to2 orb_1to2.cpp)

target_link_libraries(canny_image ${OpenCV_LIBS})
target_link_libraries(canny_stream ${OpenCV_LIBS})
target_link_libraries(blob_stream ${OpenCV_LIBS})
target_link_libraries(orb_1to2 ${OpenCV_LIBS})