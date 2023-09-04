cmake_minimum_required(VERSION 3.2)
project(visualization/pangolin)

find_package(OpenCV 4 REQUIRED)

add_executable(plot_data_monitor plot_data_monitor.cpp)
add_executable(multi_display_plot multi_display_plot.cpp)

target_link_libraries(plot_data_monitor ${THIRD_PARTY_LIBS})
target_link_libraries(multi_display_plot ${THIRD_PARTY_LIBS})
