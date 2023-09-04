add_library(tracking SHARED
    orb_extractor.cpp)

target_link_libraries(tracking ${OpenCV_LIBS})
