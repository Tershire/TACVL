// triangulate_kannala_brandt.cpp
// 2023 JUL 12
// Tershire

// referred: 

// unproject pixel points using Kannala-Brandt generic camera model
// then triangulate

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include <cmath>


// PROTOTYPE //////////////////////////////////////////////////////////////////
cv::Point3f unproject(const cv::Point2f& p2D);
Eigen::Vector3f unprojectEig(const cv::Point2f &p2D);
float TriangulateMatches(const cv::KeyPoint& keypoint1, const cv::KeyPoint& keypoint2, 
                         const Eigen::Matrix3f& R12, const Eigen::Vector3f& t12, 
                         const float SIGMA_LEVEL, Eigen::Vector3f& p3D);
void Triangulate(const cv::Point2f& p1, const cv::Point2f& p2, 
                 const Eigen::Matrix<float, 3, 4>& Tcw1, const Eigen::Matrix<float, 3, 4>& Tcw2,
                 Eigen::Vector3f& x3D);
Eigen::Vector2f project(const Eigen::Vector3f& v3D);


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
double fx = 286.5614013671875;
double fy = 286.510498046875;
double cx = 414.99539184570312;
double cy = 381.01388549804688;

double k0 = -0.013021349906921387;
double k1 =  0.053306881338357925;
double k2 = -0.049259178340435028;
double k3 =  0.0096688801422715187;

// NOT WORKING OUTSIDE main() Why (?)
// // need to check notation used in ORB-SLAM3
// Eigen::Matrix3f R12;
// R12 << 0.99999368190765381  , -0.0018149814568459988, -0.0030512968078255653,
//        0.0018118004081770778, 0.99999779462814331   , -0.001045003067702055 ,
//        0.0030531827360391617,  0.001039469032548368 ,  0.99999481439590454;

// Eigen::Vector3f t12;
// t12 << 0.063930675387382507, 0.00017283356282860041, -0.00040682472172193229;

// TriangulateMatches
const float SIGMA_LEVEL = 1; // don't know what it is. Just to test.


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Eigen Matrix & Vector for Camera Parameters ============================ 
    // need to check notation used in ORB-SLAM3
    Eigen::Matrix3f R12;
    R12 << 0.99999368190765381  , -0.0018149814568459988, -0.0030512968078255653,
           0.0018118004081770778,  0.99999779462814331  , -0.001045003067702055 ,
           0.0030531827360391617,  0.001039469032548368 ,  0.99999481439590454;

    Eigen::Vector3f t12;
    t12 << 0.063930675387382507, 0.00017283356282860041, -0.00040682472172193229;


    // Variable ===============================================================
    cv::Mat img1, img2, img1_gray, img2_gray, img1_gray_UC83, img2_gray_UC83;
    

    // Load Image =============================================================
    cv::CommandLineParser parser(argc, argv, 
                             "{@image1 | lena.jpg | input image1}"
                             "{@image2 | lena.jpg | input image2}");

    std::string img1_path = cv::samples::findFile(parser.get<cv::String>("@image1"));               
    std::string img2_path = cv::samples::findFile(parser.get<cv::String>("@image2"));
    std::string img_paths[2] = {img1_path, img2_path};

    img1 = cv::imread(img1_path, cv::IMREAD_COLOR);
    img2 = cv::imread(img2_path, cv::IMREAD_COLOR);
    cv::Mat imgs[2] = {img1, img2};

    // check loading
    for (int i = 0; i < 2; i++)
    {
        if (imgs[i].empty())
        {
            std::cout << "Could not read the image: " << img_paths[i]
                      << std::endl;
            return 1;
        }
    }

    // create a gray image
    cvtColor(img1, img1_gray, cv::COLOR_BGR2GRAY);
    cvtColor(img2, img2_gray, cv::COLOR_BGR2GRAY);
    cvtColor(img1_gray, img1_gray_UC83, cv::COLOR_GRAY2BGR);
    cvtColor(img2_gray, img2_gray_UC83, cv::COLOR_GRAY2BGR);


    // Setting ================================================================
    // GFTT (Good Features To Track)
    const int NUM_FEATURES = 150;
    static cv::Ptr<cv::GFTTDetector> gftt_detector = cv::GFTTDetector::create(NUM_FEATURES, 0.01, 20);

    // keypoints
    std::vector<cv::KeyPoint> keypoints1, keypoints2;

    std::vector<cv::Point2f> keypoints1_Point2f, keypoints2_Point2f;


    // Main ===================================================================
    // detect features in img1 ------------------------------------------------
    cv::Mat mask(img1.size(), CV_8UC1, 255);
    gftt_detector->detect(img1, keypoints1, mask);

    // check
    cv::Mat img1_check;
    cv::drawKeypoints(img1_gray_UC83, keypoints1, img1_check);


    // find features in img2 --------------------------------------------------
    // convert data type: cv::KeyPoint -> cv:Point2f
    for (cv::KeyPoint& keypoint1 : keypoints1)
    {
        keypoints1_Point2f.push_back(keypoint1.pt);

        // just to make sure    keypoints2_Point2f 
        // has the same size as keyPoints1_Point2f
        keypoints2_Point2f.push_back(keypoint1.pt);
    }

    // use LK flow to estimate points in img2
    std::vector<uchar> status;
    cv::Mat error;
    cv::calcOpticalFlowPyrLK(
        img1, img2, keypoints1_Point2f, keypoints2_Point2f, 
        status, error, cv::Size(11, 11), 3,
        cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30,
                         0.01),
        cv::OPTFLOW_USE_INITIAL_FLOW);

    // add found keypoint2_Point2f to keypoint2 if optical flow was found
    int num_keypoints_optical_flow_found = 0;
    for (size_t i = 0; i < status.size(); i++)
    {
        // if corresponding optical flow was found
        if (status[i])
        {
            keypoints2.push_back(cv::KeyPoint(keypoints2_Point2f[i], 1.f));
            num_keypoints_optical_flow_found++;
        }
        else
        {
            keypoints2.push_back(cv::KeyPoint(NAN, NAN, 1.f));
        }
    }

    // check
    std::cout << "num_keypoints_optical_flow_found: " << num_keypoints_optical_flow_found << std::endl;

    cv::Mat img2_check;
    cv::drawKeypoints(img2_gray_UC83, keypoints2, img2_check);


    // unproject --------------------------------------------------------------
    // img1
    std::vector<cv::Point3f> rays1;
    for (cv::Point2f& p2D : keypoints1_Point2f)
    {
        cv::Point3f ray1 = unproject(p2D);

        rays1.push_back(ray1);
    }

    // img2
    std::vector<cv::Point3f> rays2;
    for (cv::Point2f& p2D : keypoints2_Point2f)
    {
        cv::Point3f ray2 = unproject(p2D);

        rays2.push_back(ray2);
    }

    // triangulate (ORB-SLAM3) ------------------------------------------------
    Eigen::Vector3f p3D;
    std::vector<Eigen::Vector3f> p3Ds;
    for (size_t i = 0; i < status.size(); i++)
    {
        TriangulateMatches(keypoints1[i], keypoints2[i], 
                           R12, t12, SIGMA_LEVEL, p3D);

        p3Ds.push_back(p3D);

        std::cout << "p3D = " << p3D << std::endl;
    }

    // check
    for (size_t i = 0; i < status.size(); i++)
    {
        if (i%5 == 0)
        {
            cv::putText(img1_check,
                std::to_string(i) 
                + " (" + std::to_string(p3Ds[i](0, 0)) + ", "
                    + std::to_string(p3Ds[i](1, 0)) + ", "
                    + std::to_string(p3Ds[i](2, 0)) + ")",
                cv::Point2f(keypoints1[i].pt.x, keypoints1[i].pt.y),
                cv::FONT_HERSHEY_DUPLEX,
                0.3,
                CV_RGB(118, 185, 0),
                1);
        }
    }

    cv::Mat img_check;
    cv::hconcat(img1_check, img2_check, img_check);
    cv::imshow("{image1, image2}", img_check);
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

float TriangulateMatches(const cv::KeyPoint& keypoint1, const cv::KeyPoint& keypoint2, 
                         const Eigen::Matrix3f& R12, const Eigen::Vector3f& t12, 
                         const float SIGMA_LEVEL, Eigen::Vector3f& p3D)
{
    Eigen::Vector3f ray1 = unprojectEig(keypoint1.pt);
    Eigen::Vector3f ray2 = unprojectEig(keypoint2.pt);

    // check parallax
    Eigen::Vector3f r21 = R12 * ray2;

    const float cosParallaxRays = ray1.dot(r21)/(ray1.norm() *r21.norm());

    if(cosParallaxRays > 0.9998){
        return -1;
    }

    //Parallax is good, so we try to triangulate
    cv::Point2f p11, p22;

    p11.x = ray1[0];
    p11.y = ray1[1];

    p22.x = ray2[0];
    p22.y = ray2[1];

    Eigen::Vector3f x3D;
    Eigen::Matrix<float, 3, 4> Tcw1;
    Tcw1 << Eigen::Matrix3f::Identity(), Eigen::Vector3f::Zero();

    Eigen::Matrix<float, 3, 4> Tcw2;

    Eigen::Matrix3f R21 = R12.transpose();
    Tcw2 << R21, -R21 * t12;


    Triangulate(p11, p22, Tcw1, Tcw2, x3D);

    float z1 = x3D(2);
    if(z1 <= 0)
    {
        return -2;
    }

    float z2 = R21.row(2).dot(x3D) + Tcw2(2, 3);
    if (z2 <= 0) 
    {
        return -3;
    }

    //Check reprojection error
    Eigen::Vector2f uv1 = project(x3D);

    float errX1 = uv1(0) - keypoint1.pt.x;
    float errY1 = uv1(1) - keypoint1.pt.y;

    if((errX1*errX1+errY1*errY1) > 5.991 * SIGMA_LEVEL) // reprojection error is high
    {   
        return -4;
    }

    Eigen::Vector3f x3D2 = R21 * x3D + Tcw2.col(3);
    Eigen::Vector2f uv2 = project(x3D2);

    float errX2 = uv2(0) - keypoint2.pt.x;
    float errY2 = uv2(1) - keypoint2.pt.y;

    if((errX2*errX2+errY2*errY2) > 5.991 * SIGMA_LEVEL) // reprojection error is high
    {   
        return -5;
    }

    p3D = x3D;

    return z1;
}

void Triangulate(const cv::Point2f& p1, const cv::Point2f& p2, 
                 const Eigen::Matrix<float, 3, 4>& Tcw1, const Eigen::Matrix<float, 3, 4>& Tcw2,
                 Eigen::Vector3f& x3D)
{
    Eigen::Matrix<float, 4, 4> A;
    A.row(0) = p1.x*Tcw1.row(2) - Tcw1.row(0);
    A.row(1) = p1.y*Tcw1.row(2) - Tcw1.row(1);
    A.row(2) = p2.x*Tcw2.row(2) - Tcw2.row(0);
    A.row(3) = p2.y*Tcw2.row(2) - Tcw2.row(1);

    Eigen::JacobiSVD<Eigen::Matrix4f> svd(A, Eigen::ComputeFullV);
    Eigen::Vector4f x3Dh = svd.matrixV().col(3);
    x3D = x3Dh.head(3)/x3Dh(3);
}

Eigen::Vector2f project(const Eigen::Vector3f& x3D)
{
    const float x2_plus_y2 = x3D[0] * x3D[0] + x3D[1] * x3D[1];
    const float theta = atan2f(sqrtf(x2_plus_y2), x3D[2]);
    const float psi = atan2f(x3D[1], x3D[0]);

    const float theta2 = theta  * theta;
    const float theta3 = theta  * theta2;
    const float theta5 = theta3 * theta2;
    const float theta7 = theta5 * theta2;
    const float theta9 = theta7 * theta2;
    const float r = theta + k0 * theta3 + k1 * theta5
                          + k2 * theta7 + k3 * theta9;

    Eigen::Vector2f uv;
    uv[0] = fx * r * cos(psi) + cx;
    uv[1] = fy * r * sin(psi) + cy;

    return uv;
}
