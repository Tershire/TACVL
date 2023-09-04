cmake_minimum_required(VERSION 3.2)
project(tracking)

find_package(OpenCV 4 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# dependencies ################################################################
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

###############################################################################
add_executable(track track.cpp)
add_executable(optical_flow optical_flow.cpp)

target_link_libraries(track ${OpenCV_LIBS})
target_link_libraries(optical_flow tracking ${OpenCV_LIBS})

add_subdirectory(src)
