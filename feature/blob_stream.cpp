// canny_edge_stream.cpp
// 2023 MAY 29
// Tershire

// referred:
// https://eehoeskrap.tistory.com/123

// stream video

// command: 


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
// #include <opencv2/features2d.hpp>

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Variable ===============================================================
    Mat frame, frame_blob;

    // detector object
    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create();

    //SimpleBlobDetector::Params params;
	//params.filterByArea = true;
	//params.minArea = 4;
	//params.maxArea = 1000;
	//params.filterByColor = true;
	//params.blobColor = 0; // dark [0, 255] bright
	//params.maxThreshold = 170;
	//params.filterByCircularity = 10;
	//Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

    // key points
    std::vector<KeyPoint> keypoints;


    // Create VideoCapture Object =============================================
    VideoCapture cap(2);

    // check capture
    if (!cap.isOpened()) {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


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

        // convert color (just for eye relaxation)
        cvtColor(frame, frame, COLOR_BGR2GRAY);

        // Blob Detection -----------------------------------------------------
        // apply blur filter (to reduce noise)
        // GaussianBlur(frame, frame, Size(5, 5), 0); // effective for Size > 3

        // apply blob detector
        detector->detect(frame, keypoints);

        // draw
        drawKeypoints(frame, keypoints, frame_blob, Scalar(0, 0, 255),
                      DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        // show frame ---------------------------------------------------------
        imshow("Canny Edge Stream", frame_blob);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
