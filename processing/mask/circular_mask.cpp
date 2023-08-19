// circular_mask.cpp
// 2023 AUG 16
// Tershire

// referred:

// command:


#include "opencv2/opencv.hpp"


int main(int argc, char **argv)
{
    // setting ////////////////////////////////////////////////////////////////
    cv::Mat img_L, img_L_out;
    cv::Mat img_R, img_R_out;
    cv::Mat mask_L, mask_R;
    cv::Mat img_show;

    // load image /////////////////////////////////////////////////////////////
    cv::CommandLineParser parser(argc, argv, 
        "{@image1 | lena.jpg | input image1}"
        "{@image2 | lena.jpg | input image2}");

    std::string img_path_L = cv::samples::findFile(parser.get<cv::String>("@image1"));               
    std::string img_path_R = cv::samples::findFile(parser.get<cv::String>("@image2"));
    std::string img_paths[2] = {img_path_L, img_path_R};

    img_L = imread(img_path_L, cv::IMREAD_COLOR);
    img_R = imread(img_path_R, cv::IMREAD_COLOR);
    cv::Mat imgs[2] = {img_L, img_R};

    // check loading
    for (int i = 0; i < 2; ++i)
    {
        if (imgs[i].empty())
        {
            std::cout << "Could not read the image: " << img_paths[i]
                      << std::endl;
            return -1;
        }
    }

    // pre-processing /////////////////////////////////////////////////////////
    // convert to grayscale
    cv::cvtColor(img_L, img_L, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img_R, img_R, cv::COLOR_BGR2GRAY);

    // apply blur filter (to reduce noise)
    cv::GaussianBlur(img_L, img_L, cv::Size(5, 5), 0);
    cv::GaussianBlur(img_R, img_R, cv::Size(5, 5), 0);

    // main ///////////////////////////////////////////////////////////////////
    cv::Point IMAGE_CENTER = cv::Point(img_L.cols/2, img_L.rows/2);
    mask_L = cv::Mat(img_L.size(), CV_8UC1, cv::Scalar(0, 0, 0));
    mask_R = cv::Mat(img_R.size(), CV_8UC1, cv::Scalar(0, 0, 0));
    int radius_L = std::min(img_L.cols, img_L.rows)/2 - 60;
    int radius_R = std::min(img_R.cols, img_R.rows)/2 - 60;

    cv::circle(mask_L, IMAGE_CENTER + cv::Point(-10, -22), radius_L, cv::Scalar(255, 255, 255), cv::FILLED);
    cv::circle(mask_R, IMAGE_CENTER + cv::Point(-14, -15), radius_R, cv::Scalar(255, 255, 255), cv::FILLED);

    // test ///////////////////////////////////////////////////////////////////
    cv::Mat test_mask(img_L.size(), CV_8UC1, 255);
    cv::rectangle(test_mask, IMAGE_CENTER - cv::Point(30, 20),
                             IMAGE_CENTER + cv::Point(40, 70), 0, cv::FILLED);

    cv::bitwise_and(mask_L, test_mask, mask_L);

    // output /////////////////////////////////////////////////////////////////
    cv::bitwise_and(img_L, img_L, img_L_out, mask_L);
    cv::bitwise_and(img_R, img_R, img_R_out, mask_R);

    // show ///////////////////////////////////////////////////////////////////
    // concat
    cv::hconcat(img_L_out, img_R_out, img_show);

    // show
    cv::imshow("Circular Mask", img_show);
    int k = cv::waitKey(0);

    return 0;
}
