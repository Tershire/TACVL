cmake_minimum_required(VERSION 3.2)
project(processing/object_detection)

find_package(OpenCV 4 REQUIRED)

add_executable(yolo_cv_stream yolo_cv_stream.cpp)

target_link_libraries(yolo_cv_stream ${OpenCV_LIBS})
