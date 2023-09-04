cmake_minimum_required(VERSION 3.2)
project(feature)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# dependencies ################################################################
find_package(OpenCV 4 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/include)

###############################################################################
add_executable(blob_stream blob_stream.cpp)
add_executable(orb_1to2 orb_1to2.cpp)
add_executable(sift_1to2 sift_1to2.cpp)
add_executable(test_detection_image test_detection_image.cpp)
add_executable(test_detection_stream test_detection_stream.cpp)
add_executable(extract_ORB_octree extract_ORB_octree.cpp)
add_executable(distribute_keypoints_octree distribute_keypoints_octree.cpp)

target_link_libraries(blob_stream ${OpenCV_LIBS})
target_link_libraries(orb_1to2 ${OpenCV_LIBS})
target_link_libraries(sift_1to2 ${OpenCV_LIBS})
target_link_libraries(test_detection_image ${OpenCV_LIBS})
target_link_libraries(test_detection_stream ${OpenCV_LIBS})
target_link_libraries(extract_ORB_octree feature ${OpenCV_LIBS})
target_link_libraries(distribute_keypoints_octree feature ${OpenCV_LIBS})

add_subdirectory(src)
