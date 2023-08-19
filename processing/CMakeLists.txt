cmake_minimum_required(VERSION 3.2)
project(processing)

find_package(OpenCV 4 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(binarization)
add_subdirectory(color)
add_subdirectory(contour)
add_subdirectory(contrast)
add_subdirectory(mask)
