// qr_stream.cpp
// 2023 JUN 03
// Tershire

// referred:
// https://itchbo.tistory.com/101

// track QR code and show as video stream

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
    VideoCapture cap(2);

    // check capture
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


    // QR Detector Setting ====================================================
    QRCodeDetector qr_detector;
    std::vector<Point> points; // 2D coordinates of QR code vertices
    bool detected;


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

        // QR -----------------------------------------------------------------
        // detect
        detected = qr_detector.detect(frame, points);

        if (detected)
        {
            // draw bounding box
            polylines(frame, points, true, Scalar(0, 0, 255), 2);

            // print 2D coordinates of QR code vertices
            std::cout << points << std::endl;
        }

        // show frame ---------------------------------------------------------
        imshow("QR", frame);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
