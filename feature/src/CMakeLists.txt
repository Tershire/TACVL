add_library(feature SHARED
    orb_extractor.cpp)

target_link_libraries(feature ${OpenCV_LIBS})
