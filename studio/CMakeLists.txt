cmake_minimum_required(VERSION 3.2)
project(studio)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "-std=c++17 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE  "-std=c++17 -O3 -fopenmp -pthread -pg")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# easy-profiler
# -----------------------------------------------------------------------------
set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/easy_profiler")
# -----------------------------------------------------------------------------

# dependencies ################################################################
# OpenCV
find_package(OpenCV 4 REQUIRED)
###############################################################################

add_executable(otsu_canny_stream otsu_canny_stream.cpp)

target_link_libraries(otsu_canny_stream ${OpenCV_LIBS})

# easy-profiler
# -----------------------------------------------------------------------------
find_package(easy_profiler REQUIRED)
# -----------------------------------------------------------------------------

set(THIRD_PARTY_LIBS
    ${OpenCV_LIBS}
    easy_profiler)

# subdirectories ##############################################################
add_subdirectory(ionmav)
