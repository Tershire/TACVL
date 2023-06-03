cmake_minimum_required(VERSION 3.2)
project(Marker)

find_package(OpenCV 4 REQUIRED)

add_executable(qr_stream qr_stream.cpp)

target_link_libraries(qr_stream ${OpenCV_LIBS})