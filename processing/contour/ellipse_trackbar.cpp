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


// PROTOTYPE //////////////////////////////////////////////////////////////////
void threshold_callback(int, void*);


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
Mat img_gray, img_ellipse;

// Canny Threshold
int threshold_1 = 50;
const int MAX_THRESHOLD = 255;

// I & O
std::string image_path;
int k;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{  
    // Load Image =============================================================
    CommandLineParser parser(argc, argv, 
                             "{@input | lena.jpg | input image}");

    image_path = samples::findFile(parser.get<String>("@input"));               
    Mat img = imread(image_path, IMREAD_COLOR);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return 1;
    }


    // MAIN ===================================================================
    // convert to grayscale
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    
    // apply blur filter (to reduce noise)
    // GaussianBlur(img, img, Size(5, 5), 0); // effective for Size > 3
    blur(img_gray, img, Size(3,3)); // Sobel -> seems to work better


    // Show Image =============================================================
    const char* source_window = "Source";
    namedWindow(source_window);
    imshow(source_window, img);
    
    createTrackbar("Canny threshold:", source_window, &threshold_1, 
                   MAX_THRESHOLD, threshold_callback);

    threshold_callback(0, 0);
    
    k = waitKey();


    // Save Image =============================================================
    if (k == 's')
    {
        std::string file_dir, file_format;

        size_t index_dot = image_path.find(".");
        file_dir    = image_path.substr(0, index_dot);
        file_format = image_path.substr(index_dot, image_path.length());

        imwrite(file_dir + "_ellipse" + file_format, img_ellipse);
        std::cout << "Ellipse image saved to the original image directory"
                  << std::endl;
    }

    return 0;
}


// HELPER /////////////////////////////////////////////////////////////////////
void threshold_callback(int, void*)
{
    // Variable ===============================================================
    Mat img_edge;


    // Setting ================================================================
    std::vector<std::vector<Point>> contours;

    // drawing
    Point2f rect_points[4];


    // MAIN ===================================================================
    // apply Canny edge detector
    Canny(img_gray, img_edge, threshold_1, threshold_1 * 2);

    // find contours ----------------------------------------------------------
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

    // draw contours ----------------------------------------------------------
    img_ellipse = Mat::zeros(img_edge.size(), CV_8UC3);
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
}
