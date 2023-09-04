// plot_data_monitor.cpp
// 2023 AUG 23
// Tershire

// referred: 

// 

// command: 


#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>
#include <pangolin/display/display.h>
#include <pangolin/plot/plotter.h>
#include <unistd.h>


int main(int argc, char **argv)
{
    // example data stream ////////////////////////////////////////////////////
    std::vector<float> t_stream   = {0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10,  11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
    std::vector<float> v_x_stream = {0, 0, 0, 0, 0, 0, 1, 3,  5,  5,  4, -10,  1,  2,  7,  8,  0,  3,  0,  0,  0,  0,  0}; 
    std::vector<float> v_y_stream = {0, 0, 0, 0, 0, 0, 0, 2, -1, -2,  3,   2,  1,  0,  3, -8,  0, -1,  0,  0,  0,  0,  0};
    float speed; 

    // setting ////////////////////////////////////////////////////////////////
    // create OpenGL window in single line
    pangolin::CreateWindowAndBind("Motion Monitor", 940, 360);

    // data logger object
    pangolin::DataLog log;

    // add named labels (option)
    std::vector<std::string> labels;
    labels.push_back(std::string("v_x"));
    labels.push_back(std::string("v_y"));
    labels.push_back(std::string("speed"));
    log.SetLabels(labels);

    // OpenGL 'view' of data. We might have many views of the same data.
    pangolin::Plotter plotter(&log, 0.0f, 2.0f*(float)M_PI/0.1, -2.0f, 2.0f, (float)M_PI/(2.0f*0.1), 0.5f);
    plotter.SetBounds(0.0, 1.0, 0.0, 1.0);
    plotter.Track("$i");

    // Add some sample annotations to the plot
    plotter.AddMarker(pangolin::Marker::Vertical,  -1000, pangolin::Marker::LessThan, pangolin::Colour::Blue().WithAlpha(0.2f) );
    plotter.AddMarker(pangolin::Marker::Horizontal,  100, pangolin::Marker::GreaterThan, pangolin::Colour::Red().WithAlpha(0.2f) );
    plotter.AddMarker(pangolin::Marker::Horizontal,   10, pangolin::Marker::Equal, pangolin::Colour::Green().WithAlpha(0.2f) );
   
    // side menu setup --------------------------------------------------------
    // pangolin::CreatePanel("menu")
    //     .SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(175));

    // // data
    // pangolin::Var<double> v_x("ui.A_Double", 3, 0, 5);
    // ------------------------------------------------------------------------

    pangolin::DisplayBase().AddDisplay(plotter);

    // main ///////////////////////////////////////////////////////////////////
    int i = 0;
    while(!pangolin::ShouldQuit())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float v_x = v_x_stream[i];
        float v_y = v_y_stream[i];
        float speed = sqrt(v_x*v_x + v_y*v_y);

        log.Log(v_x, v_y, speed);
        i += 1;

        if (i >= t_stream.size()) i = 0;

        // Render graph, Swap frames and Process Events
        pangolin::FinishFrame();
    }

    return 0;
}
