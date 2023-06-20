// stream_video.cpp
// 2023 MAY 29
// Tershire

// referred:
// https://docs.opencv.org/3.4/dd/d43/tutorial_py_video_display.html
// https://docs.opencv.org/4.x/d8/dfe/classcv_1_1VideoCapture.html

// stream video

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat frame;


    // Create VideoCapture Object =============================================
    VideoCapture cap;
    // VideoCapture cap(0); // more simple & usual method

    // ------------------------------------------------------------------------
    // device & API info
    int device_id = 0;             // 0 = open default camera
    int api_id = cv::CAP_ANY;      // 0 = autodetect default API

    // open selected camera using selected API
    cap.open(device_id, api_id);
    // ------------------------------------------------------------------------

    // check capture
    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


    // Stream Video ===========================================================
    for(;;)
    {
        // read frame
        cap >> frame;
        // cap.read(frame); // equivalent classical method

        // check frame
        if (frame.empty()) 
        {
            std::cerr << "ERROR: Blank frame\n";
            break;
        }

        // show frame
        imshow("Video Stream", frame);
        int key = waitKey(10);
        if (key == 27) 
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
