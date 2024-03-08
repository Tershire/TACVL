// stream_and_save_video.cpp
// 2024 MAR 08
// Tershire

// referred:
// https://docs.opencv.org/4.x/df/d94/samples_2cpp_2videowriter_basic_8cpp-example.html

// stream and save video

// command: 


#include <iostream>

#include "opencv2/opencv.hpp"


int main(int argc, char **argv)
{
    // variable ===============================================================
    cv::Mat frame;

    // create VideoCapture object =============================================
    int camera_id;
	std::cout << "Enter camera_id: ";
	std::cin >> camera_id;

    cv::VideoCapture cap(camera_id);

    if (!cap.isOpened()) 
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return -1;
    }

    // create VideoWriter object ==============================================
    // get one frame to know frame size and type
    cap >> frame;

    if (frame.empty())
    {
        std::cerr << "ERROR: blank frame\n";
        return -1;
    }
    bool is_color_image = (frame.type() == CV_8UC3);

    
    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');
    double fps = 30;

    cv::VideoWriter writer{"my_video.mp4", codec, fps, frame.size(), is_color_image};

    if (!writer.isOpened()) 
    {
        std::cerr << "ERROR: Writer is not open\n";
        return -1;
    }

    // stream and save video ==================================================
    for(;;)
    {
        cap >> frame;

        if (frame.empty()) 
        {
            std::cerr << "ERROR: Blank frame\n";
            break;
        }

        cv::imshow("Video Stream", frame);

        writer.write(frame);

        int key = cv::waitKey(10);
        if (key == 27) 
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}
