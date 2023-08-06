// tutorial_contour.cpp
// 2023 AUG 05
// Tershire

// referred:
// https://learnopencv.com/contour-detection-using-opencv-python-c/
// https://learnopencv.com/find-center-of-blob-centroid-using-opencv-cpp-python/
// https://docs.opencv.org/4.x/dd/d49/tutorial_py_contour_features.html

// command:


#include "opencv2/opencv.hpp"


int main(int argc, char **argv)
{
    // Variable ===============================================================
    cv::Mat img, img_out;


    // Load Image =============================================================
    cv::CommandLineParser parser(argc, argv, 
        "{@input | lena.jpg | input image}");

    std::string image_path = cv::samples::findFile(parser.get<cv::String>("@input"));               
    img = imread(image_path, cv::IMREAD_COLOR);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return -1;
    }


    // Main ===================================================================
    // convert to grayscale
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        
    // apply blur filter (to reduce noise)
    cv::blur(img, img, cv::Size(3,3)); // Sobel

    // binarize
    cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

    // convert to BGR for outpur
    cv::cvtColor(img, img_out, cv::COLOR_GRAY2BGR); 

    // ------------------------------------------------------------------------
    // find contours
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //
    std::cout << "number of contours found: " << contours.size() << std::endl; 

    // filter by contour area
    double area;
    std::vector<std::vector<cv::Point>> filtered_contours;
    const int MIN_AREA = 250;
    const int MAX_AREA = img.size().width * img.size().height - 1500;
    for (auto contour : contours)
    {
        area = cv::contourArea(contour, false);
        if (area > MIN_AREA && area < MAX_AREA)
        {
            filtered_contours.push_back(contour);
        }
    }

    //
    std::cout << "number of contours after filtering: " << filtered_contours.size() << std::endl; 

    // centroid of each contour
    cv::Moments m;
    cv::Point p_centroid;
    std::vector<cv::Point> p_centroids;
    for (auto contour : filtered_contours)
    {
        m = cv::moments(contour);
        p_centroid = cv::Point(m.m10/m.m00, m.m01/m.m00);
        p_centroids.push_back(p_centroid);
    }

    // draw contours
    for (int i = 0; i < filtered_contours.size(); ++i)
    {
        cv::drawContours(img_out, filtered_contours, i, cv::Scalar(255, 0, 0));
    }

    // draw centroids
    for (auto p_centroid : p_centroids)
    {
        cv::circle(img_out, p_centroid, 2, cv::Scalar(0, 255, 0), 2);
    }


    // Show Image =============================================================
    cv::imshow("Main", img_out);
    int k = cv::waitKey(0);


    // Save Image =============================================================
    if (k == 's')
    {
        std::string file_dir, file_format;

        size_t index_dot = image_path.find(".");
        file_dir    = image_path.substr(0, index_dot);
        file_format = image_path.substr(index_dot, image_path.length());

        cv::imwrite(file_dir + "_tutorial_contour" + file_format, img_out);
        std::cout << "Output image saved to the original image directory"
                  << std::endl;
    }

    return 0;
}
