// canny_edge_stream.cpp
// 2023 MAY 29
// Tershire

// referred:

// stream video

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat frame, frame_edge;


    // Create VideoCapture Object =============================================
    VideoCapture cap(0);

    // check capture
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


    // Stream Video ===========================================================
    for(;;)
    {
        // read frame ---------------------------------------------------------
        cap >> frame;

        // check frame
        if (frame.empty()) 
        {
            std::cerr << "ERROR: Blank frame\n";
            break;
        }

        // Canny Edge Detection -----------------------------------------------
        // apply blur filter (to reduce noise)
        GaussianBlur(frame, frame, Size(5, 5), 0); // effective for Size > 3

        // apply Canny Edge Detector
        Canny(frame, frame_edge, 50, 100);

        // show frame ---------------------------------------------------------
        imshow("Canny Edge Stream", frame_edge);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
