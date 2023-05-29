// show_image.cpp
// 2023 MAY 28
// Tershire

// referred:
// https://docs.opencv.org/4.x/db/deb/tutorial_display_image.html

// load & show image then convert to grayscale.
// save the grayscale image when 's' key is down.

// command: /Feature$ build/show_image data/dove.jpg


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Load Image =============================================================
    std::string file_dir    = "Data/Dove";
    std::string file_format = ".jpg";
    
    std::string image_path = samples::findFile(file_dir + file_format);
    Mat img = imread(image_path, IMREAD_COLOR);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return 1;
    }


    // Show Image =============================================================
    imshow("Display", img);
    int k = waitKey(0);


    // Convert to Grayscale Image =============================================
    Mat img_gray;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    

    // Save Image =============================================================
    if (k == 's')
    {
        imwrite(file_dir + "_gray" + file_format, img_gray);
        std::cout << "Grayscale image saved to the original image directory\n";
    }

    return 0;
}
