// connected_components.cpp
// 2023 SEP 03
// Tershire

// referred:

// command:


#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // image //////////////////////////////////////////////////////////////////
    cv::Mat img, img_out;

    // load image /////////////////////////////////////////////////////////////
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

    const int IMAGE_WIDTH  = img.cols;
    const int IMAGE_HEIGHT = img.rows;
    const int IMAGE_AREA = IMAGE_WIDTH * IMAGE_HEIGHT;
    const int MIN_AREA = IMAGE_AREA * 0.001;

    float area;

    // pre-processing /////////////////////////////////////////////////////////
    // convert to grayscale
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);

    // ------------------------------------------------------------------------
    // convert to BGR for output
    cv::cvtColor(img, img_out, cv::COLOR_GRAY2BGR);
    // ------------------------------------------------------------------------

    cv::threshold(img, img, 180, 255, cv::THRESH_BINARY_INV);
    
    // main ///////////////////////////////////////////////////////////////////
    cv::Mat labels, stats, centroids;
    int connectivity = 8; // or 4
    int num_labels = cv::connectedComponentsWithStats(img, labels, stats, centroids, connectivity);
    for (int i = 0; i < num_labels; ++i)
    {
        int x = stats.at<int>(i, cv::CC_STAT_LEFT);
        int y = stats.at<int>(i, cv::CC_STAT_TOP);
        int w = stats.at<int>(i, cv::CC_STAT_WIDTH);
        int h = stats.at<int>(i, cv::CC_STAT_HEIGHT);
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        double cx = centroids.at<double>(i, 0);
        double cy = centroids.at<double>(i, 1);
    }

    //
    std::cout << "num_labels: " << num_labels << std::endl;

    cv::Mat mask(labels.size(), CV_8UC1, cv::Scalar(0));
    cv::Mat surfSup = stats.col(4) > MIN_AREA;

    int num_filtered_labels = 0;
    for (int i = 1; i < num_labels; ++i)
    {
        if (surfSup.at<uchar>(i, 0))
        {
            mask = mask | (labels == i);

            num_filtered_labels += 1;
        }
    }
    cv::Mat result(img.size(), CV_8UC1, cv::Scalar(0));
    img.copyTo(result, mask);

    //
    std::cout << "num_filtered_labels: " << num_filtered_labels << std::endl;

    // output /////////////////////////////////////////////////////////////////
    // draw ===================================================================
    // // centroid of each contour
    // cv::Moments m;
    // cv::Point p_centroid;
    // std::vector<cv::Point> p_centroids;
    // for (auto contour : valid_contours)
    // {
    //     m = cv::moments(contour);
    //     p_centroid = cv::Point(m.m10/m.m00, m.m01/m.m00);
    //     p_centroids.push_back(p_centroid);
    // }

    // // draw contours
    // for (int i = 0; i < valid_contours.size(); ++i)
    // {
    //     cv::drawContours(img_out, valid_contours, i, cv::Scalar(0, 255, 0));
    // }

    // // draw centroids
    // for (auto p_centroid : p_centroids)
    // {
    //     cv::circle(img_out, p_centroid, 2, cv::Scalar(0, 255, 0), 2);
    // }

    // show ===================================================================
    const int SCALE_FACTOR = 0.5;
    cv::resize(result, result, cv::Size(), SCALE_FACTOR, SCALE_FACTOR, cv::INTER_LINEAR);

    cv::imshow("result", result);
    int k = cv::waitKey(0);

    return 0;
}
