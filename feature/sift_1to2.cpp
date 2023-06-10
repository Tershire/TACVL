// orb_1to2.cpp
// 2023 MAY 31
// Tershire

// referred:
// https://docs.opencv.org/3.4/d7/d60/classcv_1_1SIFT.html
// https://docs.opencv.org/4.x/d5/d51/
// group__features2d__main.html#ga15e1361bda978d83a2bea629b32dfd3c
// https://docs.opencv.org/3.4/db/d39/classcv_1_1DescriptorMatcher.html
// https://bkshin.tistory.com/entry/
// OpenCV-28-%ED%8A%B9%EC%A7%95-%EB%A7%A4%EC%B9%ADFeature-Matching

// SIFT from one image to the other

// command: /feature$ build/sift_1to2 data/cone1.jpg data/cone2.jpg


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
// #include <opencv2/features2d/features2d.hpp> // SIFT

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Load Image =============================================================
    CommandLineParser parser(argc, argv, 
                             "{@image1 | data/cone1.jpg | input image1}"
                             "{@image2 | data/cone2.jpg | input image2}");

    std::string img_path_1 = samples::findFile(parser.get<String>("@image1"));               
    std::string img_path_2 = samples::findFile(parser.get<String>("@image2"));
    std::string img_paths[2] = {img_path_1, img_path_2};

    Mat img_1 = imread(img_path_1, IMREAD_COLOR);
    Mat img_2 = imread(img_path_2, IMREAD_COLOR);
    Mat imgs[2] = {img_1, img_2};

    // check loading
    for (int i = 0; i < 2; i++)
    {
        if (imgs[i].empty())
        {
            std::cout << "Could not read the image: " << img_paths[i]
                      << std::endl;
            return 1;
        }
    }


    // SIFT Setup =============================================================
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    Mat descriptors_1, descriptors_2;
    
    Ptr<FeatureDetector>     detector   = SIFT::create();
    Ptr<DescriptorExtractor> descriptor = SIFT::create();
    Ptr<DescriptorMatcher>   matcher    = 
        DescriptorMatcher::create("BruteForce"); // can't use Hamming-based


    // ORB Extraction =========================================================  
    // Keypoint  : detect Oriented FAST
    detector->detect(img_1, keypoints_1);
    detector->detect(img_2, keypoints_2);

    // Descriptor: compute BRIEF Descriptor
    descriptor->compute(img_1, keypoints_1, descriptors_1);
    descriptor->compute(img_2, keypoints_2, descriptors_2);

    // output -----------------------------------------------------------------
    Mat outImg_1;
    drawKeypoints(img_1, keypoints_1, outImg_1, Scalar::all(-1),
                  DrawMatchesFlags::DEFAULT);
    imshow("SIFT Features", outImg_1);


    // Feature Matching =======================================================
    // Hamming Distance -------------------------------------------------------
    std::vector<DMatch> matches;
    matcher->match(descriptors_1, descriptors_2, matches);

    // sort & remove outliers -------------------------------------------------
    // distance [min, max]
    auto min_max = minmax_element(matches.begin(), matches.end(),
        [](const DMatch& m1, const DMatch& m2) 
        {return m1.distance < m2.distance;}); // hard to understand
    double min_distance = min_max.first ->distance;
    double max_distance = min_max.second->distance;

    // remove bad matching
    // in this case, when Hamming Distance is less than twice of min
    std::vector<DMatch> good_matches;
    for (int i = 0; i < descriptors_1.rows; i++)
    {
        if (matches[i].distance <= max(2 * min_distance, 30.0)) // 30.0?
        {
            good_matches.push_back(matches[i]);
        }
    }


    // Draw Output ============================================================
    Mat img_match, img_good_match; 
    
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, 
                matches, img_match);
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, good_matches, 
                img_good_match);
    
    imshow("all matches" , img_match);
    imshow("good matches", img_good_match);
    int k = waitKey(0);
    

    // Save Image =============================================================
    std::string file_dir, file_format;

    size_t index_dot = img_path_2.find(".");
    file_dir    = img_path_2.substr(0, index_dot - 1);
    file_format = img_path_2.substr(index_dot, img_path_2.length());

    if (k == 's')
    {
        imwrite(file_dir + "_good_match" + file_format, img_good_match);
        std::cout << "Good match image saved to the original image directory\n";
    }

    return 0;
}
