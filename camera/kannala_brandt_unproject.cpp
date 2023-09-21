// kannala_brandt_unproject.cpp
// 2023 JUL 11
// Tershire

// referred: 

// unproject pixel points using Kannala-Brandt generic camera model

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
#include <eigen3/Eigen/Core>
#include <cmath>


// PROTOTYPE //////////////////////////////////////////////////////////////////
cv::Point3f unproject(const cv::Point2f& p2D);
Eigen::Vector3f unprojectEig(const cv::Point2f &p2D);


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
double fx = 286.5614013671875;
double fy = 286.510498046875;
double cx = 512 / 2; // 414.99539184570312;
double cy = 512 / 2; // 381.01388549804688;

double k0 = -0.013021349906921387;
double k1 =  0.053306881338357925;
double k2 = -0.049259178340435028;
double k3 =  0.0096688801422715187;    

// cv::Mat cameraMatrix = 
//         (cv::Mat1d(3, 3) << fx,  0, cx, 
//                              0, fy, cy, 
//                              0,  0,  1);

// cv::Mat distCoeffs = (cv::Mat1d(1, 4) << k0, k1, k2, k3);


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    cv::Mat img, img_gray, img_gray_UC83, img_unproject;
    

    // Load Image =============================================================
    cv::CommandLineParser parser(argc, argv, 
                                 "{@input | lena.jpg | input image}");

    std::string image_path = 
        cv::samples::findFile(parser.get<cv::String>("@input"));               
    img = cv::imread(image_path, cv::IMREAD_COLOR);

    // check loading
    if (img.empty())
    {
        std::cout << "Could not read the image: " << image_path
                  << std::endl;
        return 1;
    }

    // create a gray image
    cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
    cvtColor(img_gray, img_gray_UC83, cv::COLOR_GRAY2BGR); 

    // get image dimensions
    const cv::Size IMG_SIZE = img.size();
    const int HEIGHT = IMG_SIZE.height;
    const int WIDTH  = IMG_SIZE.width;
    
    // image center
    const int IMG_CX =  WIDTH/2;
    const int IMG_CY = HEIGHT/2;

    //
    std::cout << "Image Size (W, H): " << WIDTH  << ", " << HEIGHT << std::endl;
    std::cout << "Image Center     : " << IMG_CX << ", " << IMG_CY << std::endl;

    //
    // cv::imshow("img", img);
    // cv::waitKey(0);


    // Setting ================================================================
    // test point grid to unproject -------------------------------------------
    // circular pattern
    std::vector<cv::Point2f> test_grid_points;

    const int NUM_RIMS   =  5;
    const int NUM_SPOKES = 10;
    const double DELTA_R     = (std::max(HEIGHT, WIDTH) / 2) / NUM_RIMS;
    const double DELTA_ANGLE = (2 * CV_PI) / NUM_SPOKES;

    //
    std::cout << "DELTA_R    : " << DELTA_R << " [px]" << std::endl;
    std::cout << "DELTA_ANGLE: " << DELTA_ANGLE*(180/CV_PI) << " [deg]" << std::endl;

    double r, angle;
    r = 0;
    for (int i = 0; i < NUM_RIMS; i++)
    {
        r = i*DELTA_R;
        for (int j = 0; j < NUM_SPOKES; j++)
        {   
            angle = j*DELTA_ANGLE;

            // std::cout << "r    : " << r << " [px]" << std::endl;
            // std::cout << "angle: " << angle*(180/CV_PI) << " [deg]" << std::endl;

            // std::cout << "point: " << IMG_CX + r * cos(angle) 
            //                        << IMG_CY + r * sin(angle) << std::endl;

            cv::Point2f test_grid_point;
            test_grid_point.x = IMG_CX + r * cos(angle);
            test_grid_point.y = IMG_CY + r * sin(angle);

            if (test_grid_point.x <= WIDTH && test_grid_point.y <= HEIGHT)
            {
                // std::cout << "test point at: " << test_grid_point.x << ", " << test_grid_point.y << std::endl;
                test_grid_points.push_back(test_grid_point);
            }
        }
        angle = 0; 
    }

    // check: plot test grid points on image
    for (cv::Point2f& test_grid_point : test_grid_points)
    {
        cv::circle(img_gray_UC83, test_grid_point, 2, cv::Scalar(0, 255, 0), 1, 8, 0);
    }

    //
    // cv::imshow("test", img_gray_UC83);
    // cv::waitKey(0);


    // Main ===================================================================
    // unproject
    std::vector<cv::Point3f> rays;
    for (cv::Point2f& p2D : test_grid_points)
    {
        cv::Point3f ray = unproject(p2D);

        rays.push_back(ray);
    }

    // check: plot rays on image
    const int RAY_SCALE = 100; // scale to make projections of rays visible on the image
    size_t i = 0;
    for (cv::Point3f& ray : rays)
    {
        cv::Point2f ray_projected_on_img(ray.x, ray.y);

        //
        std::cout << "test grid point " << i << ": " << std::endl;
        std::cout << "[ray.x, ray.y] = " << ray_projected_on_img << std::endl;
        cv::putText(img_gray_UC83,
            std::to_string(i) 
            + " (" + std::to_string(int (atan2(ray.x, 1.0)*(180/CV_PI))) + ", "
                   + std::to_string(int (atan2(ray.y, 1.0)*(180/CV_PI))) + ")",
            test_grid_points[i],
            cv::FONT_HERSHEY_DUPLEX,
            0.3,
            CV_RGB(118, 185, 0),
            1);
        
        // scale
        ray_projected_on_img *= RAY_SCALE;
        
        // translate 
        ray_projected_on_img.x += IMG_CX;
        ray_projected_on_img.y += IMG_CY;

        cv::circle(img_gray_UC83, ray_projected_on_img, 2, cv::Scalar(0, 0, 255), 1, 8, 0);

        //
        i++;
    }

    cv::imshow("test", img_gray_UC83);
    cv::waitKey(0);


    return 0;
}


// HELPER /////////////////////////////////////////////////////////////////////
// from ORB-SLAM3 -------------------------------------------------------------
cv::Point3f unproject(const cv::Point2f& p2D)
{
    // Setting ----------------------------------------------------------------
    double const PRECISION = 1e-6;
    // ------------------------------------------------------------------------

    // use Newthon method to solve for theta with good precision (err ~ e-6)
    cv::Point2f pw((p2D.x - cx) / fx,
                   (p2D.y - cy) / fy);
    float scale = 1.f;
    float theta_d = sqrtf(pw.x * pw.x + pw.y * pw.y);
          theta_d = fminf(fmaxf(-CV_PI / 2.f, theta_d), CV_PI / 2.f);

    if (theta_d > 1e-8)
    {
        // compensate distortion iteratively
        float theta = theta_d;
        for (int j = 0; j < 10; j++)
        {
            float theta2 = theta  * theta , theta4 = theta2 * theta2, 
                  theta6 = theta4 * theta2, theta8 = theta4 * theta4;
            float k0_theta2 = k0 * theta2,
                  k1_theta4 = k1 * theta4;
            float k2_theta6 = k2 * theta6,
                  k3_theta8 = k3 * theta8;
            float theta_fix = (theta * (1 + k0_theta2 + k1_theta4 + k2_theta6 + k3_theta8) - theta_d) /
                              (1 + 3 * k0_theta2 + 5 * k1_theta4 + 7 * k2_theta6 + 9 * k3_theta8);
            theta = theta - theta_fix;
            if (fabsf(theta_fix) < PRECISION)
                break;
        }
        //scale = theta - theta_d;
        scale = std::tan(theta) / theta_d;
    }

    return cv::Point3f(pw.x * scale, pw.y * scale, 1.f);
}

Eigen::Vector3f unprojectEig(const cv::Point2f &p2D)
{
    cv::Point3f ray = unproject(p2D);
    return Eigen::Vector3f(ray.x, ray.y, ray.z);
}
