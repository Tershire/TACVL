// qr_wechat.cpp
// 2023 AUG 12
// Tershire

// referred: 
// https://learnopencv.com/wechat-qr-code-scanner-in-opencv/


#include <opencv2/opencv.hpp>
#include <opencv2/wechat_qrcode.hpp>


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img; 

    // setting ////////////////////////////////////////////////////////////////
    // capture ================================================================
    cv::VideoCapture cap(2);

    // check capture
    if (!cap.isOpened())
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return -1;
    }

    // detector ===============================================================
    cv::Ptr<cv::wechat_qrcode::WeChatQRCode> detector;
    detector = cv::makePtr<cv::wechat_qrcode::WeChatQRCode>(
        "model/detect.prototxt", 
        "model/detect.caffemodel",
        "model/sr.prototxt", 
        "model/sr.caffemodel");

    ///////////////////////////////////////////////////////////////////////////
    for (;;)
    {
        // load frame /////////////////////////////////////////////////////////
        cap >> img;

        // check frame
        if (img.empty()) 
        {
            std::cerr << "ERROR: Blank frame\n";
            break;
        }

        // main ///////////////////////////////////////////////////////////////
        // detect & decode ====================================================
        std::vector<cv::Point> vertices;
        std::vector<std::string> decoded_strings;
        // auto decoded_strings = detector->detectAndDecode(img, vertices);
        decoded_strings = detector->detectAndDecode(img, vertices);

        // draw ///////////////////////////////////////////////////////////////
        // if (!vertices.empty())
        // {
        //     cv::polylines(img, vertices, true, cv::Scalar(0, 255, 0), 2);
        // }

        for (auto& decoded_string : decoded_strings)
        {
            std::cout << "decoded_string: " << decoded_string << std::endl;
        }

        // show frame /////////////////////////////////////////////////////////
        cv::imshow("QR Wechat", img);
        int key = cv::waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
