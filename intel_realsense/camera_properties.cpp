// depth_map_stream.cpp
// 2023 JUN 18
// Tershire

// referred:
// https://github.com/IntelRealSense/librealsense/issues/1706

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include <librealsense2/rs.hpp>
#include <opencv2/opencv.hpp>


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    rs2::log_to_console(RS2_LOG_SEVERITY_ERROR);

    // Variable ===============================================================
    rs2::frameset frameset;
    
    // Setting ================================================================
    // create filter
    rs2::colorizer colorizer;
    
    // create pipeline
    rs2::pipeline pipeline;

    // Open Pipe ==============================================================
    pipeline.start();

    // get 1st frame and its dimensions
    frameset = pipeline.wait_for_frames();
        
    rs2::depth_frame depth_frame = frameset.get_depth_frame();
    rs2::frame frame_depth = depth_frame;

    const int WIDTH  = frame_depth.as<rs2::video_frame>().get_width();
    const int HEIGHT = frame_depth.as<rs2::video_frame>().get_height();

    // camera properties ======================================================
    // int width  = rs2_intrinsics.width;
    // int height = rs2_intrinsics.height;

    // float ppx = rs2_intrinsics.ppx;
    // float ppy = rs2_intrinsics.ppy;

    // float fx = rs2_intrinsics.fx;
    // float fy = rs2_intrinsics.fy;

    // rs2_distortion model = rs2_intrinsics.model;
    // float[5] coeffs = rs2_intrinsics.coeffs;

    rs2_intrinsics const intrinsics = pipeline.get_active_profile().get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>().get_intrinsics();

    int width  = intrinsics.width;
    int height = intrinsics.height;
    std::cout << "W: " << width  << std::endl;
    std::cout << "H: " << height << std::endl;

    float ppx = intrinsics.ppx;
    float ppy = intrinsics.ppy;
    std::cout << "ppx: " << ppx << std::endl;
    std::cout << "ppy: " << ppy << std::endl;

    float fx = intrinsics.fx;
    float fy = intrinsics.fy;
    std::cout << "fx: " << fx << std::endl;
    std::cout << "fy: " << fy << std::endl;

    rs2_distortion model = intrinsics.model;
    std::cout << "model: " << model << std::endl;
    
    std::vector<float> coeffs;
    for (int i = 0; i < 5; ++i)
    {
        float coeff = intrinsics.coeffs[i];
        coeffs.push_back(coeff);
        std::cout << coeff << std::endl;
    }

    // pipeline.get_active_profile().get_stream(RS2_STREAM_COLOR).as<rs2::video_stream_profile>().get_extrinsics_to(stream_profile);

    // Loop ===================================================================
    for (;;)
    {
        // read frame ---------------------------------------------------------
        frameset = pipeline.wait_for_frames();
        
        rs2::depth_frame depth_frame = frameset.get_depth_frame();
        rs2::frame frame_depth = depth_frame
            .apply_filter(colorizer);

        // convert rs2::frame to cv::mat --------------------------------------
        cv::Mat image_out = cv::Mat(cv::Size(WIDTH, HEIGHT), CV_8UC3, 
            (void*) frame_depth.get_data(), cv::Mat::AUTO_STEP);
                        
        // show frame ---------------------------------------------------------
        cv::imshow("out", image_out);
        int key = cv::waitKey(1);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return EXIT_SUCCESS;
}
