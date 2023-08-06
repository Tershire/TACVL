cmake_minimum_required(VERSION 3.2)
project(processing/contour)

find_package(OpenCV 4 REQUIRED)

add_executable(canny_image canny_image.cpp)
add_executable(canny_stream canny_stream.cpp)
add_executable(canny_trackbar canny_trackbar.cpp)
add_executable(canny_trackbar_on_gray canny_trackbar_on_gray.cpp)
add_executable(ellipse_image ellipse_image.cpp)
add_executable(ellipse_stream ellipse_stream.cpp)
add_executable(ellipse_trackbar ellipse_trackbar.cpp)
add_executable(hough_line_stream hough_line_stream.cpp)

target_link_libraries(canny_image ${OpenCV_LIBS})
target_link_libraries(canny_stream ${OpenCV_LIBS})
target_link_libraries(canny_trackbar ${OpenCV_LIBS})
target_link_libraries(canny_trackbar_on_gray ${OpenCV_LIBS})
target_link_libraries(ellipse_image ${OpenCV_LIBS})
target_link_libraries(ellipse_stream ${OpenCV_LIBS})
target_link_libraries(ellipse_trackbar ${OpenCV_LIBS})
target_link_libraries(hough_line_stream ${OpenCV_LIBS})

add_subdirectory(tutorial)
