// otsu_canny_stream.cpp
// 2023 MAY 31
// Tershire

// referred:

// convert to gray frame, apply Otsu thresholding, then Canny edge detector.
// I see no big difference from using only Canny edge detector.

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
    VideoCapture cap(2);

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

        // main ---------------------------------------------------------------
        // convert to grayscale (needed for Otsu)
        cvtColor(frame, frame, COLOR_BGR2GRAY);

        // apply blur filter (to reduce noise)
        GaussianBlur(frame, frame, Size(5, 5), 0); // effective for Size > 3

        // apply Otsu Thresholding
        threshold(frame, frame_edge, 0, 255, THRESH_OTSU);

        // apply Canny edge detector
        Canny(frame, frame_edge, 50, 100);

        // show frame ---------------------------------------------------------
        imshow("Otsu-Canny", frame_edge);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
