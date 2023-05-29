// canny_test_image.cpp
// 2023 MAY 28
// Wonhee LEE

// referred:
// https://stackoverflow.com/questions/63543033/
// does-cv2-canny-perform-a-gaussian-blur

// command: /Feature$ build/canny_test_image Data/Dove.jpg


// HEADER FILE ////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
// #include "opencv2/imgproc.hpp" // Gaussian Blur

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


    // Canny Edge Detection ===================================================
    // apply blur filter (to reduce noise)
    GaussianBlur(img, img, Size(5, 5), 0); 
    // is effective only for Kernel size > 3

    // apply Canny Edge Detector
    Canny(img, img_edge, 50, 100);


    // Show Image =============================================================
    imshow("Canny Edge Image", img_edge);
    int k = waitKey(0);


    // Save Image =============================================================
    if (k == 's')
    {
        std::string file_dir, file_format;

        size_t index_dot = image_path.find(".");
        file_dir    = image_path.substr(0, index_dot);
        file_format = image_path.substr(index_dot, image_path.length());

        imwrite(file_dir + "_Canny_edge" + file_format, img_edge);
        std::cout << "Canny edge image saved to the original image directory"
                  << std::endl;
    }

    return 0;
}
