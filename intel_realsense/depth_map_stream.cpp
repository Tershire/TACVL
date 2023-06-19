// depth_map_stream.cpp
// 2023 JUN 18
// Tershire

// referred:
// https://github.com/IntelRealSense/librealsense/blob/master/wrappers/
// opencv/imshow/rs-imshow.cpp
// python/examples/opencv_viewer_example.py

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include <librealsense2/rs.hpp>
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) try
{
    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);

    // Variable ===============================================================
    rs2::frameset data;
    rs2::frame depth_frame;
    
    // OpenCV output
    const auto WINDOW_NAME = "Output Stream";
    namedWindow(WINDOW_NAME, WINDOW_AUTOSIZE);
    Mat frame_output;


    // Setting ================================================================
    // create filter
    rs2::colorizer color_map;
    // rs2::rates_printer fps_printer;
    
    // create pipeline
    rs2::pipeline pipe;


    // Open Pipe ==============================================================
    pipe.start();

    // get 1st frame and its dimensions
    data = pipe.wait_for_frames();
        
    depth_frame = data.get_depth_frame()
                      .apply_filter(color_map);;

    const int WIDTH  = depth_frame.as<rs2::video_frame>().get_width();
    const int HEIGHT = depth_frame.as<rs2::video_frame>().get_height();

    
    // Loop ===================================================================
    while(waitKey(1) < 0 && 
          getWindowProperty(WINDOW_NAME, WND_PROP_AUTOSIZE) >= 0)
    {
        // read frame ---------------------------------------------------------
        data = pipe.wait_for_frames();
        
        depth_frame = data.get_depth_frame()
                          .apply_filter(color_map);;

        // convert rs2::frame to cv::mat --------------------------------------
        frame_output = Mat(Size(WIDTH, HEIGHT), CV_8UC3, 
                           (void*)depth_frame.get_data(), Mat::AUTO_STEP);
                        
        // show frame ---------------------------------------------------------
        imshow(WINDOW_NAME, frame_output);
        int key = waitKey(1);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return EXIT_SUCCESS;
}

// ERROR CATCH ////////////////////////////////////////////////////////////////
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " 
              << e.get_failed_function() 
              << "(" << e.get_failed_args() << "):\n    " 
              << e.what() << std::endl;
    return EXIT_FAILURE;
}

catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}
