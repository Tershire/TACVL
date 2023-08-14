cmake_minimum_required(VERSION 3.2)
project(studio/ionmav)

find_package(OpenCV 4 REQUIRED)

add_executable(decide_when_to_release decide_when_to_release.cpp)

target_link_libraries(decide_when_to_release ${THIRD_PARTY_LIBS})
