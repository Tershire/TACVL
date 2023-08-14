cmake_minimum_required(VERSION 3.2)
project(eigen_sophus)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# DEPENDENCIES ################################################################
# Eigen
include_directories("/usr/include/eigen3")

# OpenCV
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Sophus
find_package(Sophus REQUIRED)
include_directories(${Sophus_INCLUDE_DIRS})

include_directories(../include)

###############################################################################
add_executable(tutorial tutorial.cpp)

target_link_libraries(tutorial ${OpenCV_LIBS})
