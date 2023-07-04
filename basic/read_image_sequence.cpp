// read_image_sequence.cpp
// 2023 JUL 04
// Tershire

// referred: https://stackoverflow.com/questions/50210800/
//           how-to-read-files-in-sequence-from-a-directory-in-opencv-and
//           -use-it-for-processi

// load & show a sequence of unnumbered images

// command:


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Load Image =============================================================
    std::string file_dir    = "/home/tershire/Documents/SLAM_dataset/KissFly/"
                              "ovoide_1/cam0/*";
    std::string file_format = ".png";
    std::string image_path = file_dir + file_format;

    std::vector<std::string> file_names;
    cv::glob(image_path, file_names, false);


    // Show Image =============================================================
    for (size_t i = 0; i < file_names.size(); i++)
    {
        // read image
        Mat img = imread(file_names[i], IMREAD_COLOR);

        // check loading
        if (img.empty())
        {
            std::cout << "Could not read the image: " << image_path
                    << std::endl;
            return 1;
        }

        imshow("Display", img);
        int k = waitKey(1);
    }

    return 0;
}
