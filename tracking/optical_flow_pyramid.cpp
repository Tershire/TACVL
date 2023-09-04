// optical_flow.cpp
// 2023 AUG 28
// Tershire

// referred: 

//

// command: 


#include <iostream>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img1, img2;
    cv::Mat img1_out, img2_out;

    // load image /////////////////////////////////////////////////////////////
    cv::CommandLineParser parser(argc, argv, 
        "{@image1 | lena.jpg | input image1}"
        "{@image2 | lena.jpg | input image2}");

    std::string img1_path = cv::samples::findFile(parser.get<cv::String>("@image1"));               
    std::string img2_path = cv::samples::findFile(parser.get<cv::String>("@image2"));
    std::string img_paths[2] = {img1_path, img2_path};

    img1 = cv::imread(img1_path, cv::IMREAD_GRAYSCALE);
    img2 = cv::imread(img2_path, cv::IMREAD_GRAYSCALE);
    cv::Mat imgs[2] = {img1, img2};

    // check loading
    for (int i = 0; i < 2; i++)
    {
        if (imgs[i].empty())
        {
            std::cout << "Could not read the image: " << img_paths[i]
                      << std::endl;
            return -1;
        }
    }

    // create a gray image
    cv::cvtColor(img1, img1_out, cv::COLOR_GRAY2BGR);
    cv::cvtColor(img2, img2_out, cv::COLOR_GRAY2BGR);
    
    // setting ////////////////////////////////////////////////////////////////
    int num_features = 240;
    float scale_factor = 1.2; // (ORB-SLAM3) scale factor between levels in the scale pyramid 
    int num_levels = 8;

    int first_FAST_threshold = 20, second_FAST_threshold = 7; // (ORB-SLAM3)

    // main ///////////////////////////////////////////////////////////////////
    // build pyramid ==========================================================
    cv::buildOpticalFlowPyramid(img, optical_flow_pyramid,
        Size 	winSize,
        int 	maxLevel,
        bool 	withDerivatives = true,
        int 	pyrBorder = BORDER_REFLECT_101,
        int 	derivBorder = BORDER_CONSTANT,
        bool 	tryReuseInputImage = true 
        )

    // extract FAST ===========================================================
    ORB_Extractor::Ptr orb_extractor = std::make_shared<ORB_Extractor>(
        num_features, scale_factor, num_levels, first_FAST_threshold, second_FAST_threshold);

    // compute pyramid
    orb_extractor->compute_pyramid(image);
    
    // compute keypoints
    std::vector<std::vector<cv::KeyPoint>> keypoints_at_each_level;
    orb_extractor->compute_keypoints_octree(keypoints_at_each_level);

    return 0;
}

void optical_flow_single_level_CV(
    const Mat& img1,
    const Mat& img2,
    const std::vector<cv::KeyPoint>& keypoints1,
    std::vector<KeyPoint>& keypoints2,
    std::vector<bool>& statuses,
    cv::Mat& error)
{
    cv::calcOpticalFlowPyrLK(img1, img2, keypoints1, keypoints2, statuses, error);
}

void optical_flow_multi_level_CV(
    const Mat& img1,
    const Mat& img2,
    const std::vector<cv::KeyPoint>& keypoints1,
    std::vector<KeyPoint>& keypoints2,
    std::vector<bool>& success,
    bool inverse)
{
	
}