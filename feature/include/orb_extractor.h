// keypoints_octree.h

// 2023 AUG 24
// Wonhee LEE

// reference:


#ifndef FEATURE_ORBEXTRACTOR_H
#define FEATURE_ORBEXTRACTOR_H

#include <opencv2/opencv.hpp>


/**
 * 
 */
class Extractor_Node
{
public:
    Extractor_Node() {}

    void divide_node(Extractor_Node& node1, Extractor_Node& node2, Extractor_Node& node3, Extractor_Node& node4);

    std::vector<cv::KeyPoint> keypoints;
    cv::Point2i UL, UR, BL, BR;
    std::list<Extractor_Node>::iterator it;

    bool is_one_keypoint_left = false;
};

/**
 * 
 */
class ORB_Extractor
{
public:
    typedef std::shared_ptr<ORB_Extractor> Ptr;

    // constructor & destructor ///////////////////////////////////////////////
    ORB_Extractor() {}

    ORB_Extractor(const int& num_features, const float& scale_factor, 
        const int& num_levels, const int& first_FAST_threshold, const int& second_FAST_threshold);

    // getter & setter ////////////////////////////////////////////////////////
    // getter =================================================================
    std::vector<int> get_num_features_at_each_level() const {return num_features_at_each_level_;}

    // member methods /////////////////////////////////////////////////////////
    void compute_pyramid(cv::Mat image);

    void compute_keypoints_octree(std::vector<std::vector<cv::KeyPoint>>& keypoints_at_each_level);

    std::vector<cv::KeyPoint> distribute_keypoints_octree(const std::vector<cv::KeyPoint>& keypoints_to_distribute, 
        const int& min_border_x, const int& max_border_x, const int& min_border_y, const int& max_border_y, 
        const int& num_features, const int& level);

    std::vector<cv::KeyPoint> distribute_keypoints_octree(const std::vector<cv::KeyPoint>& keypoints_to_distribute, 
        const int& num_features, const int& level);

private:
    // member data ////////////////////////////////////////////////////////////
    float scale_factor_;
    int num_features_;
    int num_levels_; // number of pyramid levels
    int first_FAST_threshold_, second_FAST_threshold_;
     
    std::vector<cv::Mat> image_pyramid_;

    std::vector<int> num_features_at_each_level_;
    std::vector<float> scale_factors_for_each_level_;
    std::vector<float> inverse_scale_factors_for_each_level_;

    std::vector<float> sigma2_levels_for_each_level_; // what is this ?
    std::vector<float> inverse_sigma2_levels_for_each_level_;
};

#endif // FEATURE_ORBEXTRACTOR_H
