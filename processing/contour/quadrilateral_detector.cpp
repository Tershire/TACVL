 // quadrilateral_detector.cpp
// 2023 AUG 30
// Tershire

// referred:

// command:


#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // image //////////////////////////////////////////////////////////////////
    cv::Mat img, img_out;

    // contour ================================================================
    // morph shape types: RECT, CROSS, ELLIPSE
    cv::Mat kernel_open = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::Mat kernel_close = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::Mat kernel_dilate = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));

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

    // main ///////////////////////////////////////////////////////////////////
    // convert to grayscale
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
        
    // apply blur filter (to reduce noise)
    cv::blur(img, img, cv::Size(3, 3)); // Sobel

    // morphological transform ------------------------------------------------
    // open
    cv::morphologyEx(img, img, cv::MORPH_OPEN, kernel_open,
        cv::Point(-1, -1), 1, cv::BORDER_CONSTANT);

    // close
    cv::morphologyEx(img, img, cv::MORPH_CLOSE, kernel_close,
        cv::Point(-1, -1), 1, cv::BORDER_CONSTANT);
    // ------------------------------------------------------------------------

    // binarize
    // cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
    cv::Canny(img, img, 70, 255);

    // morphological transform ------------------------------------------------
    // dilate
    cv::dilate(img, img, kernel_dilate, cv::Point(-1, -1), 3);

    // open
    cv::morphologyEx(img, img, cv::MORPH_OPEN, kernel_open,
        cv::Point(-1, -1), 1, cv::BORDER_CONSTANT);

    // close
    cv::morphologyEx(img, img, cv::MORPH_CLOSE, kernel_close,
        cv::Point(-1, -1), 1, cv::BORDER_CONSTANT);
    // ------------------------------------------------------------------------

    // convert to BGR for output
    cv::cvtColor(img, img_out, cv::COLOR_GRAY2BGR); 

    // find contours ==========================================================
    // std::vector<std::vector<cv::Point>> contours;
    // std::vector<cv::Vec4i> hierarchy;
    // cv::findContours(img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    // //
    // std::cout << "number of contours found: " << contours.size() << std::endl; 

    // std::vector<cv::Point> approx_contour;
    // std::vector<std::vector<cv::Point>> approx_contours;
    // for (auto contour : contours)
    // {
    //     area = cv::contourArea(contour, false);

    //     double epsilon = 0.01 * cv::arcLength(contour, true);
    //     cv::approxPolyDP(contour, approx_contour, epsilon, true);

    //     if (area > MIN_AREA && approx_contour.size() == 4)
    //     {
    //         approx_contours.push_back(approx_contour);

    //         std::cout << "contour.size(): " << approx_contour.size() << std::endl;
    //     }
    // }

    // find contours: TEST ====================================================
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //
    std::cout << "number of contours found: " << contours.size() << std::endl; 

    for (auto contour : contours)
    {
        area = cv::contourArea(contour, false);

        if (area > MIN_AREA)
        {
            contours.push_back(contour);

            std::cout << "contour.size(): " << contour.size() << std::endl;
        }
    }

    //
    std::cout << "number of contours after filtering: " << contours.size() << std::endl;

    std::vector<std::vector<cv::Point>> valid_contours;
    valid_contours = contours;

    // convex hull ============================================================
    // float CONVEXITY_DEFECT_DEPTH_THRESHOLD = 1.0;

    // std::vector<cv::Point> contour;
    // // std::vector<std::vector<cv::Point>> convex_hull_points(approx_contours.size());
    // // std::vector<std::vector<int>> convex_hull_indices(approx_contours.size());
    // // std::vector<std::vector<cv::Vec4i>> convexity_defects(approx_contours.size());
    // std::vector<cv::Point> hull_point_set;
    // std::vector<int> hull_index_set;
    // std::vector<cv::Vec4i> convexity_defect_set;
    // bool is_valid_quadrilateral;
    // std::vector<std::vector<cv::Point>> valid_contours;
    // for (size_t i = 0; i < approx_contours.size(); ++i)
    // {
    //     is_valid_quadrilateral = true;

    //     contour = approx_contours[i];
    //     // cv::convexHull(cv::Mat(contour), convex_hull_points[i], false);
    //     // cv::convexHull(cv::Mat(contour), convex_hull_indices[i], false);
    //     cv::convexHull(cv::Mat(contour), hull_point_set);
    //     cv::convexHull(contour, hull_index_set);

    //     std::cout << hull_index_set[0] << std::endl;
    //     std::cout << hull_point_set[0].x << ", " << hull_point_set[0].y << std::endl;

    //     // if (convex_hull_indices[i].size() > 3)
    //     if (hull_index_set.size() > 3)
    //     {
    //         // cv::convexityDefects(contour, convex_hull_indices[i], convexity_defects[i]);
    //         cv::convexityDefects(contour, hull_index_set, convexity_defect_set);

    //         for (int j = 0; j < convexity_defect_set.size(); ++j)
    //         {
    //             float defect_depth = convexity_defect_set[j][3];
    //             if (defect_depth > CONVEXITY_DEFECT_DEPTH_THRESHOLD) is_valid_quadrilateral = false;
    //         }
            
    //         if (is_valid_quadrilateral) valid_contours.push_back(contour);
    //     }
    // }

    // //
    // std::cout << "number of valid contours: " << valid_contours.size() << std::endl;

    // output /////////////////////////////////////////////////////////////////
    // draw ===================================================================
    // centroid of each contour
    cv::Moments m;
    cv::Point p_centroid;
    std::vector<cv::Point> p_centroids;
    for (auto contour : valid_contours)
    {
        m = cv::moments(contour);
        p_centroid = cv::Point(m.m10/m.m00, m.m01/m.m00);
        p_centroids.push_back(p_centroid);
    }

    // draw contours
    for (int i = 0; i < valid_contours.size(); ++i)
    {
        cv::drawContours(img_out, valid_contours, i, cv::Scalar(0, 255, 0));
    }

    // draw centroids
    for (auto p_centroid : p_centroids)
    {
        cv::circle(img_out, p_centroid, 2, cv::Scalar(0, 255, 0), 2);
    }

    // show ===================================================================
    cv::imshow("Main", img_out);
    int k = cv::waitKey(0);

    // save ===================================================================
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
