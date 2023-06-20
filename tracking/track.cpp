// track.cpp
// 2023 JUN 06
// Tershire

// referred:
// https://learnopencv.com/object-tracking-using-opencv-cpp-python/ <*>
// https://docs.opencv.org/3.4/d2/d0a/tutorial_introduction_to_tracker.html
// https://doqtqu.tistory.com/235
// https://docs.opencv.org/4.7.0/db/dfe/classcv_1_1legacy_1_1Tracker.html
// https://docs.opencv.org/3.4/d7/d4c/classcv_1_1TrackerGOTURN.html
// https://forum.opencv.org/t/
// tracker-update-frame-bbox-compilation-failed-opencv-c/7616

// for GOTURN, goturn.prototxt, goturn.caffemodel are needed
// https://github.com/spmallick/goturn-files

// GOTURN error
// https://taktak.jp/2017/05/13/2280/

// track object based on several tracking algorithms
// if needed, user can define bounding box on the first frame

// command:


// HEADER /////////////////////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"
#include "opencv2/tracking.hpp"

using namespace cv;


// MAIN ///////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    // Pre-Setup ==============================================================
    // get user input
    CommandLineParser parser(argc, argv, 
                            "{@tracking_algo | CSRT | tracking algorithm}");
            
    std::string tracking_algo = parser.get<String>("@tracking_algo");


    // Tracker Setting ========================================================
    Ptr<Tracker> tracker;

    // if (tracking_algo == "Boosting")
    //     tracker = legacy::TrackerBoosting::create();
    if (tracking_algo == "CSRT")
    {
        tracker = TrackerCSRT::create();
        std::cout << "Tracker: CSRT\n" << std::endl;
    }
    if (tracking_algo == "GOTURN")
    {
        tracker = TrackerGOTURN::create();
        std::cout << "Tracker: GOTURN\n" << std::endl;
    }
    // if (tracking_algo == "KCF")
    //     tracker = TrackerKCF::create();
    // if (tracking_algo == "MEDIANFLOW")
    //     tracker = legacy::TrackerMedianFlow::create();
    // if (tracking_algo == "MIL")
    //     tracker = TrackerMIL::create();
    // if (tracking_algo == "MOSSE")
    //     tracker = legacy::TrackerMOSSE::create();
    // if (tracking_algo == "TLD")
    //     tracker = legacy::TrackerTLD::create();

    bool detected;


    // Variable ===============================================================
    Mat frame;


    // Create VideoCapture Object =============================================
    VideoCapture cap(0);

    // check capture
    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return 1;
    }


    // Bounding Box Selection & Initial Frame =================================
    // get initial frame
    cap >> frame;

    // check frame
    if (frame.empty()) 
    {
        std::cerr << "ERROR: Blank frame\n";
    }

    // bounding box setting
    Rect bbox; // Rect2d not working
    bbox = Rect(280, 30, 80, 320); // dafault bounding box 
    // (if not provided, error)
    // bbox = selectROI(frame, false);

    imshow("tracking", frame);

    // user-defined bounding box
    if (tracking_algo == "CSRT")
    {
        bbox = selectROI(frame, false);
        imshow("tracking", frame);
    }

    // intialize tracker
    tracker->init(frame, bbox);


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

        // track --------------------------------------------------------------
        // detect
        detected = tracker->update(frame, bbox);

        if (detected)
        {
            // draw bounding box
            rectangle(frame, bbox, Scalar(0, 0, 255), 2, 1);
        }

        // show frame ---------------------------------------------------------
        imshow("tracking", frame);
        int key = waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
