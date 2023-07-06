// undistort.cpp
// 2023 JUL 05
// Tershire

// referred: 

// undistort series of images and save

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
const bool SHOW_IMG = false, SAVE_IMG = true;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat img_undistorted;
    

    // Load Image =============================================================
    std::string file_dir    = "/home/tershire/Documents/SLAM_dataset/KissFly/"
                              "ovoide_1/cam1/*";
    std::string file_format = ".png";
    std::string image_path = file_dir + file_format;

    // TEST
    std::cout << image_path << std::endl;

    std::vector<std::string> file_names;
    glob(image_path, file_names, false);


    // Setting ================================================================
    // variable & constant
    Mat cameraMatrix = 
        (Mat1d(3, 3) << 286.5614013671875, 0, 414.99539184570312, 
                        0, 286.510498046875, 381.01388549804688, 
                        0, 0, 1);

    Mat distCoeffs = (Mat1d(1, 4) << -0.013021349906921387, 
                                      0.053306881338357925, 
                                     -0.049259178340435028, 
                                      0.0096688801422715187);

    Mat new_cameraMatrix;

    // read first image
    Mat img = imread(file_names[0], IMREAD_COLOR);

    //
    std::cout << file_names[0] << std::endl;

    //
    imshow("img", img);
    waitKey(0);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return 1;
    }

    // get image dimensions
    const cv::Size IMG_SIZE = img.size();

    // TEST
    std::cout << IMG_SIZE << std::endl;

    // get new camera matrix
    new_cameraMatrix = getOptimalNewCameraMatrix(cameraMatrix,
                                                 distCoeffs,
                                                 IMG_SIZE,
                                                 1,
                                                 IMG_SIZE,
                                                 0);

    // TEST
    std::cout << new_cameraMatrix << std::endl;


    // Loop ===================================================================
    for (size_t i = 0; i < file_names.size(); i++)
    {
        // read image
        img = imread(file_names[i], IMREAD_COLOR);

        // check loading
        if (img.empty())
        {
            std::cout << "Could not read the image: " << image_path
                      << std::endl;
            return 1;
        }

        // undistort
        undistort(img, img_undistorted, cameraMatrix, distCoeffs, 
                                        new_cameraMatrix);
        // show image
        if (SHOW_IMG == true)
        {
            imshow("img", img);
            imshow("img_undistorted", img_undistorted);
            waitKey(0);
        }

        // Save Image =========================================================
        if (SAVE_IMG == true)
        {   
            size_t index_slash = file_names[i].find_last_of("/");
            size_t index_dot   = file_names[i].find(".");

            std::string file_root_name = 
                        file_names[i].substr(index_slash + 1, index_dot);

            index_dot   = file_root_name.find(".");
            file_root_name = 
                        file_root_name.substr(0, index_dot);

            file_dir    = "/home/tershire/Documents/SLAM_dataset/KissFly/"
                          "ovoide_1_undistorted/cam1/"; 
            file_format = ".png";

            imwrite(file_dir + file_root_name + "_undistorted" + file_format, 
                    img_undistorted);
        }
    }

    return 0;
}
