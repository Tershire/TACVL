cmake_minimum_required(VERSION 3.2)
project(intel_realsense)

find_package(realsense2 REQUIRED)
find_package(OpenCV 4 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(depth_map_stream depth_map_stream.cpp)

target_link_libraries(depth_map_stream ${realsense2_LIBRARY} ${OpenCV_LIBS})
