// detect_ORB.cpp
// 2023 AUG 09
// Tershire

// referred:


#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img, img_out;

    // load image /////////////////////////////////////////////////////////////
    cv::CommandLineParser parser(argc, argv, 
        "{@image | lena.jpg | input image1}");

    std::string img_path = cv::samples::findFile(parser.get<cv::String>("@image"));               

    img = imread(img_path, cv::IMREAD_COLOR);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << img_path << std::endl;
        return -1;
    }

    // setting ////////////////////////////////////////////////////////////////
    // ORB ==================================================================== 
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::FeatureDetector> detector = nullptr;

    // main ///////////////////////////////////////////////////////////////////
    // select detector ========================================================
    // ORB
    // detector = cv::ORB::create();

    // GFTT
    detector = cv::GFTTDetector::create();

    // detect features ========================================================
    // timer ------------------------------------------------------------------
    auto t1 = std::chrono::steady_clock::now();
    // ------------------------------------------------------------------------

    detector->detect(img, keypoints);

    // timer ------------------------------------------------------------------
    auto t2 = std::chrono::steady_clock::now();
    auto time_used =
        std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "cost time: " << time_used.count() << " seconds." << std::endl;
    // ------------------------------------------------------------------------

    // output /////////////////////////////////////////////////////////////////
    cv::drawKeypoints(img, keypoints, img_out, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);

    cv::imshow("ORB Features", img_out);
    cv::waitKey(0);

    return 0;
}
