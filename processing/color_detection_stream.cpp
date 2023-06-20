// color_detection_stream.cpp
// 2023 JUN 20
// Tershire

// referred: 
// https://dsbook.tistory.com/166
// https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html
// https://docs.opencv.org/4.x/df/d9d/tutorial_py_colorspaces.html

// detect colors using cv.inRange()

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


    // Setting: Color Detection ===============================================
    // boundary (HSV) -------------------------------------------------------
    // lane: <BLUE> plane 
    Scalar lower_boundary_1(14, 100, 100)
    upper_boundary_1

    // lane: <GREEN> fixed window
    lower_boundary_2
    upper_boundary_2

    // lane: <RED> moving window
    lower_boundary_3(14)
    upper_boundary_3

    // mask
    


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
        Canny(frame_gray, frame_edge, 50, 100);

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
