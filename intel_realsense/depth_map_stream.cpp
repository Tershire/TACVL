// depth_map_stream.cpp
// 2023 JUN 18
// Tershire

// referred:

// 

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include <librealsense2/rs.hpp>
#include "opencv2/opencv.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat frame, frame_output;


    // Stream Video ===========================================================
    for(;;)
    {
        // read frame ---------------------------------------------------------


        // check frame


        // main ---------------------------------------------------------------


        // show frame ---------------------------------------------------------
        imshow("Output Stream", frame_output);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
