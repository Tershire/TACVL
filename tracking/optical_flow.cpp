// optical_flow.cpp
// 2023 AUG 28
// Tershire

// referred: 

//

// command: 


#include <iostream>
#include <math.h>
#include <vector>
#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // NAN TEST ///////////////////////////////////////////////////////////////
    cv::Mat error = (cv::Mat_<double>(3, 3) << 0, 1, 2, NAN, 3, -1, 0, -2, -4);
    double optical_flow_error = cv::mean(error).val[0];
    std::cout << "OptiFlow error: " << optical_flow_error << std::endl;

    // variable ///////////////////////////////////////////////////////////////
    error.at<double>()
    
    // load image /////////////////////////////////////////////////////////////


    // setting ////////////////////////////////////////////////////////////////

    // main ///////////////////////////////////////////////////////////////////


    return 0;
}
