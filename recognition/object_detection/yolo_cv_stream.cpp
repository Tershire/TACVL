// yolo_cv_stream.cpp
// 2023 AUG 18
// Tershire

// referred:
// https://learnopencv.com/object-detection-using-yolov5-and-opencv-dnn-in-c-and-python/
// https://learnopencv.com/how-to-run-inference-using-tensorrt-c-api/
// https://learnopencv.com/deep-learning-based-object-detection-using-yolov3-with-opencv-python-c/
// https://github.com/omair18/Tiny-Yolov3-OpenCV-Cpp/blob/master/object_detection_yolo.cpp


#include <opencv2/opencv.hpp>

// prototype
std::vector<std::string> get_output_layer_names(const cv::dnn::Net& net);

void post_process(cv::Mat& img, const std::vector<cv::Mat>& output_blobs,
    const double& CONFIDENCE_THRESHOLD, const double& NMS_THRESHOLD,
    std::vector<int>& class_IDs,
    std::vector<float>& confidences,
    std::vector<cv::Rect>& boxes);

void draw_prediction(cv::Mat& img, int left, int top, int right, int bottom, 
    int class_ID, float confidence);


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    cv::Mat img, img_out;

    // setting ////////////////////////////////////////////////////////////////
    // capture ================================================================
    cv::VideoCapture cap(0);

    // check capture
    if (!cap.isOpened())
    {
        std::cerr << "ERROR: Capturer is not open\n";
        return -1;
    }

    // neural net =============================================================
    std::string cfg_path, weights_path;
    cfg_path     = "model/yolov3-tiny.cfg";
    weights_path = "model/yolov3-tiny.weights";

    // load network
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(cfg_path, weights_path);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // check info.
    std::vector<std::string> output_layer_names = 
        get_output_layer_names(net);
    std::cout << "output layer names: " << std::endl;
    for (auto& name : output_layer_names)
    {
        std::cout << name << std::endl;
    }
    
    // output =================================================================
    const float RESIZE_SCALE = 1.0F;

    const double CONFIDENCE_THRESHOLD = 0.3;
    const double NMS_THRESHOLD = 0.2;

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

        // main ===============================================================
        // set input
        net.setInput(cv::dnn::blobFromImage(img, 1/255.0, cv::Size(416, 416), cv::Scalar(0,0,0), true, false));
        
        // main
        std::vector<cv::Mat> output_blobs;
        net.forward(output_blobs, output_layer_names);

        // post-processing
        std::vector<int> class_IDs;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;
        img_out = img.clone();
        post_process(img_out, output_blobs,
            CONFIDENCE_THRESHOLD, NMS_THRESHOLD,
            class_IDs, confidences, boxes);

        // postprocess(img_out, output_blobs);

        // show frame /////////////////////////////////////////////////////////
        // resize
        cv::resize(img_out, img_out, cv::Size(), RESIZE_SCALE, RESIZE_SCALE, cv::INTER_LINEAR);

        // ====================================================================
        cv::imshow("OpenCV YOLO", img_out);
        int key = cv::waitKey(10);
        if (key == 27)
        {
            break; // quit when 'esc' pressed
        }
    }

    return 0;
}

// Get the names of the output layers
std::vector<std::string> get_output_layer_names(const cv::dnn::Net& net)
{
    static std::vector<std::string> output_layer_names;
    if (output_layer_names.empty())
    {
        // get the names of all layers in the network
        std::vector<std::string> layer_names = net.getLayerNames();

        // get indices of the output layers
        std::vector<int> output_layer_IDs = net.getUnconnectedOutLayers();
         
        // get output layer names filtered from the all layer names
        output_layer_names.resize(output_layer_IDs.size());
        for (int i = 0; i < output_layer_IDs.size(); ++i)
        {
            output_layer_names[i] = layer_names[output_layer_IDs[i] - 1];
        }
    }
    return output_layer_names;
}

void post_process(cv::Mat& img, const std::vector<cv::Mat>& output_blobs,
    const double& CONFIDENCE_THRESHOLD, const double& NMS_THRESHOLD,
    std::vector<int>& class_IDs,
    std::vector<float>& confidences,
    std::vector<cv::Rect>& boxes)
{
    for (size_t i = 0; i < output_blobs.size(); ++i)
    {
        cv::Mat output_blob = output_blobs[i];
        float* data = (float*) output_blob.data;
        for (int j = 0; j < output_blob.rows; ++j)
        {
            /// get scores, which are located from column 5
            cv::Mat scores = output_blob.row(j).colRange(5, output_blob.cols);
            
            // get value & location of max score (class confidence)
            double confidence;
            cv::Point class_ID_point;
            cv::minMaxLoc(scores, 0, &confidence, 0, &class_ID_point);
            if (confidence > CONFIDENCE_THRESHOLD)
            {
                int center_x = int(data[0] * img.cols);
                int center_y = int(data[1] * img.rows);
                int width  = int(data[2] * img.cols);
                int height = int(data[3] * img.rows);
                int left = center_x - width  / 2;
                int top  = center_y - height / 2;
                
                class_IDs.push_back(class_ID_point.x);
                confidences.push_back(float(confidence)); 
                boxes.push_back(cv::Rect(left, top, width, height)); 
            }
            data += output_blob.cols;
        }
    }
    
    // Non-Maximum Suppression
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, CONFIDENCE_THRESHOLD, NMS_THRESHOLD, indices);

    //
    std::cout << indices.size() << std::endl;

    // draw
    int index;
    for (size_t i = 0; i < indices.size(); ++i)
    {
        index = indices[i];
        cv::Rect box = boxes[index];

        draw_prediction(img, box.x, box.y, box.x + box.width, box.y + box.height, class_IDs[index], confidences[index]);
    }
}

void draw_prediction(cv::Mat& img, int left, int top, int right, int bottom, 
    int class_ID, float confidence)
{
    //
    std::cout << "box: " << "(" << left  << ", " << top    << ")" 
                            "(" << right << ", " << bottom << ")" << std::endl;

    cv::rectangle(img, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 255, 0), 2);
    
    //Get the label for the class name and its confidence
    // string label = format("%.2f", conf);
    // if (!classes.empty())
    // {
    //     CV_Assert(classId < (int)classes.size());
    //     label = classes[classId] + ":" + label;
    // }
    
    // //Display the label at the top of the bounding box
    // int baseLine;
    // Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    // top = max(top, labelSize.height);
    // rectangle(frame, Point(left, top - round(1.5*labelSize.height)), Point(left + round(1.5*labelSize.width), top + baseLine), Scalar(255, 255, 255), FILLED);
    // putText(frame, label, Point(left, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0,0,0),1);
}
