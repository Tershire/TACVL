cmake_minimum_required(VERSION 3.2)
project(visualization)

# set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
# set(CMAKE_CXX_FLAGS_RELEASE  "-std=c++17 -O3 -fopenmp -pthread")

# list(APPEND CMAKE_MODULE_PATH ../cmake)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# DEPENDENCIES ################################################################
# Eigen
include_directories("/usr/include/eigen3")

# OpenCV
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# pangolin
find_package(Pangolin REQUIRED)
include_directories(${Pangolin_INCLUDE_DIRS})

# # Sophus
# find_package(Sophus REQUIRED)
# include_directories(${Sophus_INCLUDE_DIRS})

set(THIRD_PARTY_LIBS
    ${OpenCV_LIBS}
    # ${Sophus_LIBRARIES}
    ${Pangolin_LIBRARIES} GL GLU GLEW glut)

###############################################################################

add_subdirectory(pangolin)
