// color_picker_HSV_to_BGR.cpp
// 2023 AUG 21
// Tershire

// referred: 

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// PROTOTYPE //////////////////////////////////////////////////////////////////
void gray_values_HSV_callback(int, void*);
void put_data(Mat);


// GLOBAL CONSTANT & VARIABLE /////////////////////////////////////////////////
const int WIDTH  = 480;
const int HEIGHT = 640;

// display
const char* window_HSV = "HSV";
const char* window_BGR = "BGR";


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
Mat color_BGR;

int gray_value_H = 0, gray_value_S = 0, gray_value_V = 0;
Mat color_HSV = Mat(WIDTH, HEIGHT, CV_8UC3, 
                    Scalar(gray_value_H, gray_value_S, gray_value_V));

const int MAX_GRAY_VALUE = 255;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{  
    // MAIN ===================================================================
    // convert to BGR
    cvtColor(color_HSV, color_BGR, COLOR_HSV2BGR);


    // Show Image =============================================================
    namedWindow(window_HSV);
    namedWindow(window_BGR);
    
    createTrackbar("gray_value_H:", window_HSV, &gray_value_H, 
                   MAX_GRAY_VALUE, gray_values_HSV_callback);
    createTrackbar("gray_value_S:", window_HSV, &gray_value_S, 
                   MAX_GRAY_VALUE, gray_values_HSV_callback);
    createTrackbar("gray_value_V:", window_HSV, &gray_value_V, 
                   MAX_GRAY_VALUE, gray_values_HSV_callback);

    gray_values_HSV_callback(0, 0); // calling callback for the first time
    
    int k = waitKey();

    return 0;
}


// HELPER /////////////////////////////////////////////////////////////////////
void gray_values_HSV_callback(int, void*)
{
    // MAIN ===================================================================
    color_HSV = Mat(WIDTH, HEIGHT, CV_8UC3, 
                    Scalar(gray_value_H, gray_value_S, gray_value_V));
    
    // convert color
    cvtColor(color_HSV, color_BGR, COLOR_HSV2BGR);


    // Show Image =============================================================
    // put data as text
    put_data(color_HSV);
    put_data(color_BGR);

    // show
    imshow(window_HSV, color_HSV);
    imshow(window_BGR, color_BGR);
}

void put_data(Mat img)
{
    // extract data from the (0, 0) pixel
    Vec3b data = img.at<Vec3b>(0, 0);
    int val1 = data.val[0]; // H | B
    int val2 = data.val[1]; // S | G
    int val3 = data.val[2]; // V | R

    putText(img,
            "(" + std::to_string(val1) + ", " 
                + std::to_string(val2) + ", "
                + std::to_string(val3) + ")",
            Point(10, img.rows / 2),
            FONT_HERSHEY_DUPLEX,
            1.0,
            CV_RGB(118, 185, 0),
            2);
}