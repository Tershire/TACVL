// decide_when_to_release.cpp
// 2023 AUG 06
// Wonhee LEE

////////////////////////////
// IONLAB >< ISAE-SUPAERO //
// DRONE COMPETITION TEAM //
////////////////////////////

// referred:

// command:


#include <opencv2/opencv.hpp>
#include <easy/profiler.h>


int main(int argc, char **argv)
{
    // profiler ---------------------------------------------------------------
    EASY_MAIN_THREAD;
    EASY_PROFILER_ENABLE;
    profiler::startListen();

    EASY_BLOCK("main()");
    // ------------------------------------------------------------------------

    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img, img_out;

    // capturer ///////////////////////////////////////////////////////////////
    // const int CAMERA_ID = 0;
    // cv::VideoCapture cap(CAMERA_ID);

    std::string video_file_path;
    // video_file_path = "/home/tershire/Documents/Research/ionmav/20230806_release_test.mp4";
    video_file_path = "/media/tershire/Samsung_T3/VIDEO/ionmav/20230806_release_test.mp4";
    // video_file_path = "/media/tershire/Samsung_T3/VIDEO/Train/映画 銀河鉄道９９９　特報・劇場用予告篇.mp4";
    cv::VideoCapture cap(video_file_path);

    // check capture
    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return -1;
    }

    // get FPS
    double fps = cap.get(cv::CAP_PROP_FPS);

    //
    std::cout << "FPS: " << fps << std::endl; 

    // contour setting ////////////////////////////////////////////////////////
    // load one frame for setting =============================================
    cap >> img;

    // check frame
    if (img.empty()) 
    {
        std::cerr << "ERROR: Blank frame\n";
    }

    // min and max area [pixels] to filter ====================================
    double area;
    const int MIN_AREA = 5000;
    const int MAX_AREA = img.size().width * img.size().height - 2500;

    // centroid setting =======================================================
    cv::Point p_centroid;
    
    // loop ///////////////////////////////////////////////////////////////////
    for(;;)
    {
        // load frame =========================================================
        cap >> img;

        // check frame
        if (img.empty()) 
        {
            std::cerr << "ERROR: Blank frame\n";
            break;
        }
        
        // pre-processing =====================================================
        // convert to grayscale
        cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
            
        // apply blur filter (to reduce noise)
        cv::blur(img, img, cv::Size(3,3)); // Sobel

        // binarize
        cv::adaptiveThreshold(img, img, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);
        // cv::Canny(img, img, 30, 255);

        // --------------------------------------------------------------------
        // convert to BGR for outpur
        cv::cvtColor(img, img_out, cv::COLOR_GRAY2BGR);
        // --------------------------------------------------------------------

        // main ===============================================================
        // find contours
        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(img, contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

        //
        std::cout << "number of contours found: " << contours.size() << std::endl; 

        // filter by contour area
        EASY_BLOCK("filter contours", profiler::colors::Red);
        std::vector<std::vector<cv::Point>> filtered_contours;
        for (int i = 0; i < contours.size(); ++i)
        {
            area = cv::contourArea(contours[i], false);
            if (area > MIN_AREA && area < MAX_AREA)
            {
                filtered_contours.push_back(contours[i]);

                //
                std::cout << "filtered contour index (i): " << i << std::endl;
            }
        }
        EASY_END_BLOCK;

        //
        std::cout << "number of contours after filtering: " << filtered_contours.size() << std::endl; 

        // centroid of each contour
        EASY_BLOCK("get centroids", profiler::colors::Blue);
        cv::Moments m;
        std::vector<cv::Point> p_centroids;
        for (auto& contour : filtered_contours)
        {
            m = cv::moments(contour);
            p_centroid = cv::Point(m.m10/m.m00, m.m01/m.m00);
            p_centroids.push_back(p_centroid);
        }
        EASY_END_BLOCK;
        
        // find fit ellipses
        EASY_BLOCK("find fit ellipses", profiler::colors::Green);
        std::vector<cv::RotatedRect> fit_ellipses(filtered_contours.size());
        cv::RotatedRect fit_ellipse;
        for (auto& filtered_contour : filtered_contours)
        {        
            if(filtered_contour.size() > 5)
            {
                fit_ellipse = cv::fitEllipse(filtered_contour);
                fit_ellipses.push_back(fit_ellipse);
            }
        }
        EASY_END_BLOCK;

        //
        std::cout << "number of fit ellipses: " << fit_ellipses.size() << std::endl; 

        // draw ---------------------------------------------------------------
        EASY_BLOCK("draw contours and centroids", profiler::colors::Purple);
        for (int i = 0; i < filtered_contours.size(); ++i)
        {  
            // contour
            cv::drawContours(img_out, filtered_contours, i, cv::Scalar(255, 0, 0), 4);

            // centroid
            cv::circle(img_out, p_centroids[i], 2, cv::Scalar(0, 255, 0), 20);
        }
        EASY_END_BLOCK;
        // --------------------------------------------------------------------
        EASY_BLOCK("draw fit ellipses", profiler::colors::White);
        for (auto& fit_ellipse : fit_ellipses)
        {
            // fit ellipse
            cv::ellipse(img_out, fit_ellipse, cv::Scalar(0, 255, 0), 2);
        }
        EASY_END_BLOCK;
        // --------------------------------------------------------------------

        // Show Image =========================================================
        // resize
        const float SCALE_FACTOR = 0.5;
        cv::resize(img_out, img_out, cv::Size(), SCALE_FACTOR, SCALE_FACTOR, cv::INTER_LINEAR);

        cv::imshow("decide_when_to_release", img_out);
        int key = cv::waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }

        EASY_END_BLOCK;
    }

    return 0;
}
