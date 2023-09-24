cmake_minimum_required(VERSION 3.2)
project(basic)

# set(CMAKE_BUILD_TYPE "Release")
# set(CMAKE_CXX_FLAGS "-std=c++17 -O2")

find_package(OpenCV 4 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(show_image show_image.cpp)
add_executable(stream_video stream_video.cpp)
add_executable(read_image_sequence read_image_sequence.cpp)
add_executable(nan_inf nan_inf.cpp)
add_executable(txt_IO txt_IO.cpp)
add_executable(formatting formatting.cpp)

target_link_libraries(show_image ${OpenCV_LIBS})
target_link_libraries(stream_video ${OpenCV_LIBS})
target_link_libraries(read_image_sequence ${OpenCV_LIBS})
target_link_libraries(nan_inf ${OpenCV_LIBS})
target_link_libraries(txt_IO ${OpenCV_LIBS})
target_link_libraries(formatting)
