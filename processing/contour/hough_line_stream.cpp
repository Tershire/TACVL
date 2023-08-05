// hough_line_stream.cpp
// 2023 JUN 19
// Tershire

// referred: 
// https://docs.opencv.org/3.4/d9/db0/tutorial_hough_lines.html

// hough line detection

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat frame, frame_edge, frame_gray, frame_gray_UC83;
        

    // Create VideoCapture Object =============================================
    VideoCapture cap(2);

    // check capture
    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


    // Setting: Hough Line Transform ==========================================
    std::vector<Vec2f> lines;

    // property
    float rho, theta;
    Point pt_1, pt_2;
    double a, b;
    double x0, y0;


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

        // Main ---------------------------------------------------------------
        // convert to grayscale
        cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
        cvtColor(frame_gray, frame_gray_UC83, COLOR_GRAY2BGR);
        
        // apply blur filter (to reduce noise)
        // GaussianBlur(frame, frame, Size(5, 5), 0);
        blur(frame_gray, frame_gray, Size(3,3));

        // apply Canny edge detector
        Canny(frame_gray, frame_edge, 120, 255);

        // Hough line transform
        HoughLines(frame_edge, lines, 1, CV_PI/180, 150, 0, 0);

        // draw contours ------------------------------------------------------
        for(int i = 0; i < lines.size(); i++)
        {
            rho   = lines[i][0];
            theta = lines[i][1];
            a     = cos(theta);
            b     = sin(theta);
            x0    = a*rho;
            y0    = b*rho;

            pt_1.x = cvRound(x0 + 1000*(-b));
            pt_1.y = cvRound(y0 + 1000*( a));
            pt_2.x = cvRound(x0 - 1000*(-b));
            pt_2.y = cvRound(y0 - 1000*( a));

            // line(frame_edge_BGR, pt_1, pt_2, Scalar(0, 0, 255), 3, LINE_AA);
            line(frame_gray_UC83, pt_1, pt_2, Scalar(0, 0, 255), 3, LINE_AA);
        }

        // show frame ---------------------------------------------------------
        imshow("Hough Line Stream", frame_gray_UC83);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
