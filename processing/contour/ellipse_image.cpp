// ellipse_stream.cpp
// 2023 JUN 10
// Tershire

// referred: 
// https://docs.opencv.org/4.x/de/d62/tutorial_bounding_rotated_ellipses.html
// https://docs.opencv.org/3.4/db/dd6/classcv_1_1RotatedRect.html#details

// find ellipse after contour extraction

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat img, img_edge;
    

    // Load Image =============================================================
    CommandLineParser parser(argc, argv, 
                             "{@input | lena.jpg | input image}");

    std::string image_path = samples::findFile(parser.get<String>("@input"));               
    img = imread(image_path, IMREAD_COLOR);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return 1;
    }


    // Setting ================================================================
    std::vector<std::vector<Point>> contours;

    // drawing
    Point2f rect_points[4];

    // Canny Threshold
    const int THRESHOLD_1 = 50;


    // MAIN ===================================================================
    // convert to grayscale
    cvtColor(img, img, COLOR_BGR2GRAY);
    
    // apply blur filter (to reduce noise)
    // GaussianBlur(img, img, Size(5, 5), 0); // effective for Size > 3
    blur(img, img, Size(3,3)); // Sobel -> seems to work better

    // apply Canny edge detector
    Canny(img, img_edge, THRESHOLD_1, THRESHOLD_1 * 2);

    // find contours
    findContours(img_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE,
                    Point(0, 0));

    std::vector<RotatedRect> minRects   (contours.size());
    std::vector<RotatedRect> minEllipses(contours.size());
    for (int i = 0; i < contours.size(); i++)
    {
        minRects[i] = minAreaRect(contours[i]);
        
        if(contours[i].size() > 5) // why 5?
        {
            minEllipses[i] = fitEllipse(contours[i]);
        }
    }

    // draw contours
    Mat img_ellipse = Mat::zeros(img_edge.size(), CV_8UC3);
    for (int i = 0; i < contours.size(); i++)
    {
        drawContours(img_ellipse, contours, i, Scalar(0, 0, 255));

        ellipse(img_ellipse, minEllipses[i], Scalar(0, 255, 0), 2);

        minRects[i].points(rect_points);
        for (int j = 0; j < 4; j++)
        {
            line(img_ellipse, rect_points[j], rect_points[(j + 1) % 4], 
                    Scalar(255, 0, 0));
        }
    }


    // Show Image =============================================================
    imshow("Ellipse Image", img_ellipse);
    int k = waitKey(0);


    // Save Image =============================================================
    if (k == 's')
    {
        std::string file_dir, file_format;

        size_t index_dot = image_path.find(".");
        file_dir    = image_path.substr(0, index_dot);
        file_format = image_path.substr(index_dot, image_path.length());

        imwrite(file_dir + "_ellipse" + file_format, img_edge);
        std::cout << "Ellipse image saved to the original image directory"
                  << std::endl;
    }

    return 0;
}
