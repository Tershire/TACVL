// clahe_stream.cpp
// 2023 AUG 14
// Tershire

// referred:
// https://stackoverflow.com/questions/24341114/simple-illumination-correction-in-images-opencv-c


#include <opencv2/opencv.hpp>


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img, img_out;
    cv::Mat img_LAB, img_gray;

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
    cv::Ptr<cv::CLAHE> clahe;
    clahe = cv::createCLAHE(70, cv::Size(8, 8));

    // output =================================================================
    const float RESIZE_SCALE = 0.35;

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
        // pre-processing =====================================================
        // convert to LAB
        cv::cvtColor(img, img_LAB, cv::COLOR_BGR2Lab);
        
        // Extract img into 3 channels of LAB
        std::vector<cv::Mat> lab_channels(3);
        cv::split(img_LAB, lab_channels);

        // convert to gray for comparison -------------------------------------
        cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
        // --------------------------------------------------------------------

        // CLAHE ==============================================================
        clahe->apply(lab_channels[0], img_out);

        // show frame /////////////////////////////////////////////////////////
        // concat & resize ====================================================
        cv::Mat img_show;
        cv::hconcat(img_gray, img_out, img_show);

        // resize
        cv::resize(img_show, img_show, cv::Size(), RESIZE_SCALE, RESIZE_SCALE, cv::INTER_LINEAR);

        // ====================================================================
        cv::imshow("GRAY | CLAHE", img_show);
        int key = cv::waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
