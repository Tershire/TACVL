// otsu_image.cpp
// 2023 JUN 12
// Tershire

// referred:

// command:


// HEADER FILE ////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
// #include "opencv2/imgproc.hpp" // Gaussian Blur

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat img, img_otsu;


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


    // Otsu Thresholding ======================================================
    // convert to grayscale (needed for Otsu)
    cvtColor(img, img, COLOR_BGR2GRAY);

    // apply blur filter (to reduce noise)
    // GaussianBlur(frame, frame, Size(5, 5), 0);

    // apply Otsu Thresholding
    threshold(img, img_otsu, 0, 255, THRESH_OTSU);


    // Show Image =============================================================
    imshow("Otsu Image", img_otsu);
    int k = waitKey(0);


    // Save Image =============================================================
    if (k == 's')
    {
        std::string file_dir, file_format;

        size_t index_dot = image_path.find(".");
        file_dir    = image_path.substr(0, index_dot);
        file_format = image_path.substr(index_dot, image_path.length());

        imwrite(file_dir + "_Canny_otsu" + file_format, img_otsu);
        std::cout << "Canny otsu image saved to the original image directory"
                  << std::endl;
    }

    return 0;
}
