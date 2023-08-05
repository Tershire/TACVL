// canny_trackbar_on_gray.cpp
// 2023 JUN 28
// Tershire

// referred: 

// canny edge with threshold trackbar on gray image

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// PROTOTYPE //////////////////////////////////////////////////////////////////
void threshold_callback(int, void*);


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
Mat img_gray, img_edge;

// Canny Threshold
int lower_threshold = 50;
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
    blur(img_gray, img_gray, Size(3,3)); // Sobel -> seems to work better


    // Show Image =============================================================
    const char* source_window = "Source";
    namedWindow(source_window);
    imshow(source_window, img);
    
    createTrackbar("Canny threshold:", source_window, &lower_threshold, 
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

        imwrite(file_dir + "_edge" + file_format, img_edge);
        std::cout << "edge image saved to the original image directory"
                  << std::endl;
    }

    return 0;
}


// HELPER /////////////////////////////////////////////////////////////////////
void threshold_callback(int, void*)
{
    // Variable ===============================================================
    Mat img_edge, img_gray_UC83;
    cvtColor(img_gray, img_gray_UC83, COLOR_GRAY2BGR); 
    // ^ create new img_gray_UC83 at each call (global variable doesn't work) 
    

    // Setting ================================================================
    std::vector<std::vector<Point>> contours;


    // MAIN ===================================================================
    // apply Canny edge detector
    Canny(img_gray, img_edge, lower_threshold, lower_threshold * 2);

    // find contours
    findContours(img_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE,
                 Point(0, 0));

    // draw contours
    for (int i = 0; i < contours.size(); i++)
    {
        drawContours(img_gray_UC83, contours, i, Scalar(0, 0, 255));
    }


    // Show Image =============================================================
    imshow("edge Image", img_gray_UC83);
}
