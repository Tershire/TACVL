// canny_trackbar.cpp
// 2023 JUN 12
// Tershire

// referred: 

// canny edge with threshold trackbar

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// PROTOTYPE //////////////////////////////////////////////////////////////////
void threshold_callback(int, void*);


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
Mat img_gray, img_edge;

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
    Mat img_edge;


    // MAIN ===================================================================
    // apply Canny edge detector
    Canny(img_gray, img_edge, threshold_1, threshold_1 * 2);


    // Show Image =============================================================
    imshow("edge Image", img_edge);
}
