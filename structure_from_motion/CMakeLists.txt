cmake_minimum_required(VERSION 3.22)
project(structure_from_motion)

# set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")

include_directories(${Pangolin_INCLUDE_DIRS})

find_package(OpenCV 4 REQUIRED)
find_package(Pangolin REQUIRED)

add_executable(pose_viewer pose_viewer.cpp)

target_link_libraries(pose_viewer ${Pangolin_LIBRARIES})

add_subdirectory(triangulation)
