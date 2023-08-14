cmake_minimum_required(VERSION 3.2)
project(marker/qr)

find_package(OpenCV 4 REQUIRED)

add_executable(qr_stream qr_stream.cpp)
add_executable(qr_localize qr_localize.cpp)
add_executable(qr_wechat qr_wechat.cpp)

target_link_libraries(qr_stream ${OpenCV_LIBS})
target_link_libraries(qr_localize ${OpenCV_LIBS})
target_link_libraries(qr_wechat ${OpenCV_LIBS})
