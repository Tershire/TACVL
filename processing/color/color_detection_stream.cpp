// color_detection_stream.cpp
// 2023 JUN 20
// Tershire

// referred: 
// https://dsbook.tistory.com/166
// https://docs.opencv.org/3.4/da/d97/tutorial_threshold_inRange.html
// https://docs.opencv.org/4.x/df/d9d/tutorial_py_colorspaces.html
// https://stackoverflow.com/questions/48109650/
// how-to-detect-two-different-colors-using-cv2-inrange-in-python-opencv
// https://github.com/Amirag96/Red-color-detection/blob/master/red.py *
// https://sanpreetai.medium.com/
// detection-of-a-specific-color-red-using-opencv-and-python-fbb608ef2fed

// detect colors using cv.inRange()

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
const float SCALE_FACTOR = 0.3;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat frame, frame_HSV;

    // show
    Mat mask, output_frame;
    
    std::vector<Mat> masks, output_frames;


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
    // try BGR
    const int TOLERANCE = 15;

    // <BLUE>
    Scalar lower_boundary_1(120 - 1.4*TOLERANCE, 100, 100);
    Scalar upper_boundary_1(120 + 1.4*TOLERANCE, 255, 255);

    // <GREEN>
    Scalar lower_boundary_2( 60 - 1.5*TOLERANCE,  80,  80);
    Scalar upper_boundary_2( 60 + 1.5*TOLERANCE, 255, 255);

    // <RED>
    Scalar lower_boundary_3( 20 - 1.2*TOLERANCE, 100, 100);
    Scalar upper_boundary_3( 20 + 1.2*TOLERANCE, 255, 255);

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
        Mat output_frame_1, output_frame_2, output_frame_3;
        // ^ if these frames are declared outside the loop, image trails remain

        bitwise_and(frame, frame, output_frame_1, mask_1);
        bitwise_and(frame, frame, output_frame_2, mask_2);
        bitwise_and(frame, frame, output_frame_3, mask_3);
        
        // show frame ---------------------------------------------------------
        // concatenate output frames
        masks = {mask_1, mask_2, mask_3};
        hconcat(masks, mask);
        output_frames = {output_frame_1, output_frame_2, output_frame_3};
        hconcat(output_frames, output_frame);

        // resize
        resize(frame, frame, Size(), 
               SCALE_FACTOR, SCALE_FACTOR, INTER_LINEAR);
        resize(mask, mask, Size(), 
               SCALE_FACTOR, SCALE_FACTOR, INTER_LINEAR);
        resize(output_frame, output_frame, Size(), 
               SCALE_FACTOR, SCALE_FACTOR, INTER_LINEAR);

        //
        imshow("view", frame);
        imshow("mask", mask);
        imshow("color detection", output_frame);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
