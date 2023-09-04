// distribute_keypoints_octree.cpp
// 2023 AUG 24
// Tershire

// referred: 

// test ORB-SLAM3's keypoint computing mechanism

// command: 


#include <iostream>

#include "orb_extractor.h"


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat image, image_out_CV, image_out_ORBSLAM3;
    
    // load image /////////////////////////////////////////////////////////////
    cv::CommandLineParser parser(argc, argv, 
                                 "{@input | lena.jpg | input image}");

    std::string image_path = 
        cv::samples::findFile(parser.get<cv::String>("@input"));               
    image = cv::imread(image_path, cv::IMREAD_GRAYSCALE);

    // check loading
    if (image.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return -1;
    }

    assert(image.type() == CV_8UC1);

    // setting ////////////////////////////////////////////////////////////////
    int num_features = 240;
    float scale_factor = 1.2; // (ORB-SLAM3) scale factor between levels in the scale pyramid 
    int num_levels = 8;

    int first_FAST_threshold = 20, second_FAST_threshold = 7; // (ORB-SLAM3)

    // ------------------------------------------------------------------------
    cv::Ptr<cv::FeatureDetector> detector_;
    
    // main ///////////////////////////////////////////////////////////////////
    // OpenCV =================================================================
    detector_ = cv::ORB::create(num_features, scale_factor, num_levels, 19);

    std::vector<cv::KeyPoint> keypoints_CV;
    detector_->detect(image, keypoints_CV);

    // ORB-SLAM3 ==============================================================
    ORB_Extractor::Ptr orb_extractor = std::make_shared<ORB_Extractor>(
        num_features, scale_factor, num_levels, first_FAST_threshold, second_FAST_threshold);

    // compute pyramid
    orb_extractor->compute_pyramid(image);
    
    // distribute keypoints
    std::vector<cv::KeyPoint> keypoints_to_distribute = keypoints_CV;
    int level = 0;
    int num_features_at_level = orb_extractor->get_num_features_at_each_level()[level];
    num_features_at_level = 70; // maybe I can directly choose what I want now
    std::vector<cv::KeyPoint> distributed_keypoints = orb_extractor->distribute_keypoints_octree(keypoints_to_distribute, num_features_at_level, level);

    // show ///////////////////////////////////////////////////////////////////
    image_out_CV       = image.clone();
    image_out_ORBSLAM3 = image.clone();
    
    cv::drawKeypoints(image_out_CV, keypoints_CV, image_out_CV, cv::Scalar(0, 0, 255),
            cv::DrawMatchesFlags::DEFAULT);
    cv::drawKeypoints(image_out_ORBSLAM3, distributed_keypoints, image_out_ORBSLAM3, cv::Scalar(0, 255, 0),
            cv::DrawMatchesFlags::DEFAULT);

    cv::Mat image_show;
    cv::hconcat(image_out_CV, image_out_ORBSLAM3, image_show);
    cv::imshow("Keypoint Distribution Octree: Before | After", image_show);
    int k = cv::waitKey(0);

    return 0;
}
