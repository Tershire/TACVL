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
    Mat frame, frame_HSV, output_frame_1, output_frame_2, output_frame_3,
        output_frame;
        
    std::vector<Mat> output_frames;


    // Create VideoCapture Object =============================================
    VideoCapture cap(2);

    // check capture
    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


    // Setting: Color Detection ===============================================
    // boundary (HSV) ---------------------------------------------------------
    int tolerance = 25;

    // <BLUE>
    Scalar lower_boundary_1(120 - tolerance, 100, 100);
    Scalar upper_boundary_1(120 + tolerance, 255, 255);

    // <GREEN>
    Scalar lower_boundary_2( 60 - tolerance, 100, 100);
    Scalar upper_boundary_2( 60 + tolerance, 255, 255);

    // <RED>
    Scalar lower_boundary_3(  0 - tolerance, 100, 100);
    Scalar upper_boundary_3(  0 + tolerance, 255, 255);

    //
    Mat mask_1, mask_2, mask_3;

    
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
        // apply blur filter (to reduce noise)
        // GaussianBlur(frame, frame, Size(5, 5), 0);
        blur(frame, frame, Size(3,3));

        // convert to HSV
        cvtColor(frame, frame_HSV, COLOR_BGR2HSV);

        // mask
        inRange(frame_HSV, lower_boundary_1, upper_boundary_1, mask_1);
        inRange(frame_HSV, lower_boundary_2, upper_boundary_2, mask_2);     
        inRange(frame_HSV, lower_boundary_3, upper_boundary_3, mask_3);

        // bitwise-&
        bitwise_and(frame_HSV, frame_HSV, output_frame_1, mask_1);
        bitwise_and(frame_HSV, frame_HSV, output_frame_2, mask_2);
        bitwise_and(frame_HSV, frame_HSV, output_frame_3, mask_3);
        
        // show frame ---------------------------------------------------------
        // concatenate output frames
        output_frames = {output_frame_1, output_frame_2, output_frame_3};
        hconcat(output_frames, output_frame);

        // resize
        resize(frame, frame, Size(), 0.3, 0.3, INTER_LINEAR);
        resize(output_frame, output_frame, Size(), 0.3, 0.3, INTER_LINEAR);

        //
        imshow("view", frame);
        imshow("color detection", output_frame);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
