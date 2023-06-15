// ellipse_stream.cpp
// 2023 JUN 10
// Tershire

// referred: 
// https://docs.opencv.org/4.x/de/d62/tutorial_bounding_rotated_ellipses.html
// https://docs.opencv.org/3.4/db/dd6/classcv_1_1RotatedRect.html#details

// find ellipse after contour extraction

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


    // Setting ================================================================
    std::vector<std::vector<Point>> contours;

    // drawing
    Point2f rect_points[4];


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

        // Main --------------------------------------------------------------
        // convert to grayscale
        cvtColor(frame, frame, COLOR_BGR2GRAY);
        
        // apply blur filter (to reduce noise)
        // GaussianBlur(frame, frame, Size(5, 5), 0);
        blur(frame, frame, Size(3,3)); // Sobel -> seems to work better

        // apply Canny edge detector
        Canny(frame, frame_edge, 50, 100);

        // find contours
        findContours(frame_edge, contours, RETR_TREE, CHAIN_APPROX_SIMPLE,
                     Point(0, 0));

        std::vector<RotatedRect> minRects   (contours.size());
        std::vector<RotatedRect> minEllipses(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
            minRects[i] = minAreaRect(contours[i]);
            
            if(contours[i].size() > 25) // why 5?
            {
                minEllipses[i] = fitEllipse(contours[i]);
            }
        }

        // draw contours
        Mat frame_ellipse = Mat::zeros(frame_edge.size(), CV_8UC3);
        for (int i = 0; i < contours.size(); i++)
        {
            drawContours(frame_ellipse, contours, i, Scalar(0, 0, 255));

            ellipse(frame_ellipse, minEllipses[i], Scalar(0, 255, 0), 2);

            minRects[i].points(rect_points);
            for (int j = 0; j < 4; j++)
            {
                line(frame_ellipse, rect_points[j], rect_points[(j + 1) % 4], 
                     Scalar(255, 0, 0));
            }
        }

        // show frame ---------------------------------------------------------
        imshow("Ellipse Stream", frame_ellipse);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
