
// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/opencv.hpp>   // Include OpenCV API

static cv::Mat frame_to_Mat(const rs2::frame& f);
static cv::Mat depth_frame_to_depth_Mat(const rs2::depth_frame& f);


int main(int argc, char * argv[]) try
{
    using namespace cv;
    using namespace rs2;

    // Define colorizer and align processing-blocks
    colorizer colorize;
    align align_to(RS2_STREAM_COLOR);

    // Start the camera
    pipeline pipe;
    pipe.start();

    const auto window_name = "Display Image";
    namedWindow(window_name, WINDOW_AUTOSIZE);

    // We are using StructuringElement for erode / dilate operations
    auto gen_element = [](int erosion_size)
    {
        return getStructuringElement(MORPH_RECT,
            Size(erosion_size + 1, erosion_size + 1),
            Point(erosion_size, erosion_size));
    };

    const int erosion_size = 3;
    auto erode_less = gen_element(erosion_size);
    auto erode_more = gen_element(erosion_size * 2);

    // The following operation is taking grayscale image,
    // performs threashold on it, closes small holes and erodes the white area
    auto create_mask_from_depth = [&](Mat& depth, int thresh, ThresholdTypes type)
    {
        threshold(depth, depth, thresh, 255, type);
        dilate(depth, depth, erode_less);
        erode(depth, depth, erode_more);
    };

    // Skips some frames to allow for auto-exposure stabilization
    for (int i = 0; i < 10; i++) pipe.wait_for_frames();

    while (waitKey(1) < 0 && getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0)
    {
        frameset data = pipe.wait_for_frames();
        // Make sure the frameset is spatialy aligned 
        // (each pixel in depth image corresponds to the same pixel in the color image)
        frameset aligned_set = align_to.process(data);
        frame depth = aligned_set.get_depth_frame();
        auto color_mat = frame_to_Mat(aligned_set.get_color_frame());

        // Colorize depth image with white being near and black being far
        // This will take advantage of histogram equalization done by the colorizer
        colorize.set_option(RS2_OPTION_COLOR_SCHEME, 2);
        frame bw_depth = depth.apply_filter(colorize);

        // Generate "near" mask image:
        auto near = frame_to_Mat(bw_depth);
        cvtColor(near, near, COLOR_BGR2GRAY);
        // Take just values within range [180-255]
        // These will roughly correspond to near objects due to histogram equalization
        create_mask_from_depth(near, 180, THRESH_BINARY);

        // Generate "far" mask image:
        auto far = frame_to_Mat(bw_depth);
        cvtColor(far, far, COLOR_BGR2GRAY);
        far.setTo(255, far == 0); // Note: 0 value does not indicate pixel near the camera, and requires special attention 
        create_mask_from_depth(far, 100, THRESH_BINARY_INV);

        // GrabCut algorithm needs a mask with every pixel marked as either:
        // BGD, FGB, PR_BGD, PR_FGB
        Mat mask;
        mask.create(near.size(), CV_8UC1); 
        mask.setTo(Scalar::all(GC_BGD)); // Set "background" as default guess
        mask.setTo(GC_PR_BGD, far == 0); // Relax this to "probably background" for pixels outside "far" region
        mask.setTo(GC_FGD, near == 255); // Set pixels within the "near" region to "foreground"

        // Run Grab-Cut algorithm:
        Mat bgModel, fgModel; 
        grabCut(color_mat, mask, Rect(), bgModel, fgModel, 1, GC_INIT_WITH_MASK);

        // Extract foreground pixels based on refined mask from the algorithm
        Mat3b foreground = Mat3b::zeros(color_mat.rows, color_mat.cols);
        color_mat.copyTo(foreground, (mask == GC_FGD) | (mask == GC_PR_FGD));
        imshow(window_name, foreground);
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception& e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}

/**
 * conversion from realsense frame to cv::Mat
 * copied from Intel RealSense library
 */
static cv::Mat frame_to_Mat(const rs2::frame& f)
{
    using namespace cv;
    using namespace rs2;

    auto vf = f.as<video_frame>();
    const int w = vf.get_width();
    const int h = vf.get_height();

    if (f.get_profile().format() == RS2_FORMAT_BGR8)
    {
        return Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_RGB8)
    {
        auto r_rgb = Mat(Size(w, h), CV_8UC3, (void*)f.get_data(), Mat::AUTO_STEP);
        Mat r_bgr;
        cvtColor(r_rgb, r_bgr, COLOR_RGB2BGR);
        return r_bgr;
    }
    else if (f.get_profile().format() == RS2_FORMAT_Z16)
    {
        return Mat(Size(w, h), CV_16UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_Y8)
    {
        return Mat(Size(w, h), CV_8UC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }
    else if (f.get_profile().format() == RS2_FORMAT_DISPARITY32)
    {
        return Mat(Size(w, h), CV_32FC1, (void*)f.get_data(), Mat::AUTO_STEP);
    }

    throw std::runtime_error("Frame format is not supported yet!");
}

/** 
 * conversion from realsense depth frame to a matrix of doubles with distances in meters
 */
static cv::Mat depth_frame_to_depth_Mat(const rs2::depth_frame& f)
{
    cv::Mat dm = frame_to_Mat(f);
    dm.convertTo(dm, CV_64F);
    dm = dm * f.get_units();
    return dm;
}