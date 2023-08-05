cmake_minimum_required(VERSION 3.2)
project(marker)

find_package(OpenCV 4 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(qr)
