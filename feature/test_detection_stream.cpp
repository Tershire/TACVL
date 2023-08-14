// detect_ORB.cpp
// 2023 AUG 09
// Tershire

// referred:


#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img, img_out;
    cv::Mat image_L, image_R;

    // setting ////////////////////////////////////////////////////////////////
    // dataset ================================================================
    std::string dataset_directory_path = "/home/tershire/Documents/SLAM_dataset/KissFly/ovoide_1";

    std::vector<std::string> file_paths_L, file_paths_R;

    cv::glob(dataset_directory_path + "/cam0/*.png", file_paths_L, false);
    cv::glob(dataset_directory_path + "/cam1/*.png", file_paths_R, false);

    int num_images = std::min(file_paths_L.size(), file_paths_R.size());

    // detector ===============================================================
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::FeatureDetector> detector = nullptr;

    // select detector
    // ORB
    // detector = cv::ORB::create();

    // GFTT
    detector = cv::GFTTDetector::create();

    ///////////////////////////////////////////////////////////////////////////
    int current_image_index = 0;
    for (;;)
    {
        // load image /////////////////////////////////////////////////////////
        if (current_image_index < num_images)
        {
            // read image
            image_L = cv::imread(file_paths_L[current_image_index], cv::IMREAD_GRAYSCALE);
        }

        // check loading
        if (image_L.data == nullptr)
        {
            std::cout << "ERROR: cannot find images at index " << current_image_index << std::endl;
            return -1;
        }      

        img = image_L;

        // main ///////////////////////////////////////////////////////////////
        // detect features ====================================================
        // timer --------------------------------------------------------------
        auto t1 = std::chrono::steady_clock::now();
        // --------------------------------------------------------------------

        detector->detect(img, keypoints);

        // timer --------------------------------------------------------------
        auto t2 = std::chrono::steady_clock::now();
        auto time_used =
            std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        std::cout << "cost time: " << time_used.count() << " seconds." << std::endl;
        // --------------------------------------------------------------------

        // output /////////////////////////////////////////////////////////////
        cv::drawKeypoints(img, keypoints, img_out, cv::Scalar(0, 255, 0), cv::DrawMatchesFlags::DEFAULT);

        cv::imshow("ORB Features", img_out);
        int key = cv::waitKey(0);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }

        // increment //////////////////////////////////////////////////////////
        current_image_index++;
    }

    return 0;
}
