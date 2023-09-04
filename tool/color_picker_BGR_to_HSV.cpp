// color_picker_BGR_to_HSV.cpp
// 2023 JUN 20
// Tershire

// referred: 
// https://stackoverflow.com/questions/35737032/
// convert-a-single-color-with-cvtcolor
// https://stackoverflow.com/questions/4337902/
// how-to-fill-opencv-image-with-one-solid-color
// https://stackoverflow.com/questions/22257477/
// how-to-get-value-of-specific-pixel-after-converting-to-hsv

// 

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"

using namespace cv;


// PROTOTYPE //////////////////////////////////////////////////////////////////
void gray_values_BGR_callback(int, void*);
void put_data(Mat);


// GLOBAL CONSTANT & VARIABLE /////////////////////////////////////////////////
const int WIDTH  = 480;
const int HEIGHT = 640;

// display
const char* window_BGR = "BGR";
const char* window_HSV = "HSV";


// GLOBAL VARIABLE & CONSTANT /////////////////////////////////////////////////
Mat color_HSV;

int gray_value_B = 0, gray_value_G = 0, gray_value_R = 0;
Mat color_BGR = Mat(WIDTH, HEIGHT, CV_8UC3, 
                    Scalar(gray_value_B, gray_value_G, gray_value_R));

const int MAX_GRAY_VALUE = 255;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{  
    // MAIN ===================================================================
    // convert to HSV
    cvtColor(color_BGR, color_HSV, COLOR_BGR2HSV);


    // Show Image =============================================================
    namedWindow(window_BGR);
    namedWindow(window_HSV);
    
    createTrackbar("gray_value_B:", window_BGR, &gray_value_B, 
                   MAX_GRAY_VALUE, gray_values_BGR_callback);
    createTrackbar("gray_value_G:", window_BGR, &gray_value_G, 
                   MAX_GRAY_VALUE, gray_values_BGR_callback);
    createTrackbar("gray_value_R:", window_BGR, &gray_value_R, 
                   MAX_GRAY_VALUE, gray_values_BGR_callback);

    gray_values_BGR_callback(0, 0); // calling callback for the first time
    
    int k = waitKey();

    return 0;
}


// HELPER /////////////////////////////////////////////////////////////////////
void gray_values_BGR_callback(int, void*)
{
    // MAIN ===================================================================
    color_BGR = Mat(WIDTH, HEIGHT, CV_8UC3, 
                    Scalar(gray_value_B, gray_value_G, gray_value_R));
    
    // convert color
    cvtColor(color_BGR, color_HSV, COLOR_BGR2HSV);


    // Show Image =============================================================
    // put data as text
    put_data(color_BGR);
    put_data(color_HSV);

    // show
    imshow(window_BGR, color_BGR);
    imshow(window_HSV, color_HSV);
}

void put_data(Mat img)
{
    // extract data from the (0, 0) pixel
    Vec3b data = img.at<Vec3b>(0, 0);
    int val1 = data.val[0]; // B | H
    int val2 = data.val[1]; // G | S
    int val3 = data.val[2]; // R | V

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