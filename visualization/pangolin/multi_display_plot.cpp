// plot_data_monitor.cpp
// 2023 AUG 23
// Tershire

// referred: 

// 

// command: 


#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Core>
#include <pangolin/pangolin.h>


int main(int argc, char **argv)
{
    // example data stream ////////////////////////////////////////////////////
    std::vector<float> t_stream   = {0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10,  11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22};
    std::vector<float> v_x_stream = {0, 0, 0, 0, 0, 0, 1, 3,  5,  5,  4, -10,  1,  2,  7,  8,  0,  3,  0,  0,  0,  0,  0}; 
    std::vector<float> v_y_stream = {0, 0, 0, 0, 0, 0, 0, 2, -1, -2,  3,   2,  1,  0,  3, -8,  0, -1,  0,  0,  0,  0,  0};
    float speed; 

    // setting ////////////////////////////////////////////////////////////////
    // create OpenGL window in single line
    pangolin::CreateWindowAndBind("Multi Motion Monitor", 1024, 768);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ========================================================================
    // define projection and initial ModelView matrix
    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024, 768, 400, 400, 512, 384, 0.1, 1000),
        pangolin::ModelViewLookAt(0, -5, -10, 0, 0, 0, 0.0, -1.0, 0.0));

    // create interactive view in window
    pangolin::Handler3D handler(s_cam);
    // pangolin::View& d_cam = pangolin::CreateDisplay()
        // .SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f)
        // .SetHandler(&handler);

    // ========================================================================
    // data logger object
    pangolin::DataLog log;

    // add named labels (option)
    std::vector<std::string> labels;
    labels.push_back(std::string("v_x"));
    labels.push_back(std::string("v_y"));
    labels.push_back(std::string("speed"));
    log.SetLabels(labels);

    // ========================================================================
    // add named OpenGL viewport to window and provide 3D Handler
    pangolin::View& d_cam = pangolin::Display("view")
        .SetBounds(0.0, 1.0, 0.0, 1.0)
        .SetAspect(1024.0f / 480.0f)
        .SetHandler(&handler);
        
    // OpenGL 'view' of data. We might have many views of the same data.
    // pangolin::Plotter plotter(&log, 0.0f, 2.0f*(float)M_PI/0.1, -2.0f, 2.0f, (float)M_PI/(2.0f*0.1), 0.5f);
    pangolin::Plotter plotter(&log, 0.0, 300, -1, 1, 30, 0.5);
    // plotter.SetAspect(1024.0f / 480.0f);
    // plotter.SetBounds(0.0, pangolin::Attach::Pix(175), 0.0, 1.0);
    plotter.Track("$i");

    // Add some sample annotations to the plot
    plotter.AddMarker(pangolin::Marker::Vertical,  -1000, pangolin::Marker::LessThan, pangolin::Colour::Blue().WithAlpha(0.2f) );
    plotter.AddMarker(pangolin::Marker::Horizontal,  100, pangolin::Marker::GreaterThan, pangolin::Colour::Red().WithAlpha(0.2f) );
    plotter.AddMarker(pangolin::Marker::Horizontal,   10, pangolin::Marker::Equal, pangolin::Colour::Green().WithAlpha(0.2f) );

    // ========================================================================
    
    // pangolin::DisplayBase().AddDisplay(plotter);

    pangolin::Display("Multi Monitor")
        .SetBounds(0.0, 1.0, 0.0, 1.0)
        .SetLayout(pangolin::LayoutVertical)
        .AddDisplay(d_cam)
        .AddDisplay(plotter);

    // side menu setup --------------------------------------------------------
    pangolin::CreatePanel("menu")
        .SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(175));

    // toggle
    pangolin::Var<bool>      toggle_pause("menu.Pause"     , true , true );
    pangolin::Var<bool> button_next_frame("menu.Next Frame", false, false);
    // ------------------------------------------------------------------------

    // main ///////////////////////////////////////////////////////////////////
    int i = 0;
    while(!pangolin::ShouldQuit())
    {
        // clear screen and activate view to render into
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // set background color
        glClearColor(0.15F, 0.15F, 0.15F, 0.0F);

        // side menu operation ------------------------------------------------
        bool do_pause_, step_to_next_frame_;
        if (toggle_pause)
        {
            do_pause_ = true;
        }
        else
        {
            do_pause_ = false;
        }

        if (pangolin::Pushed(button_next_frame))
        {
            step_to_next_frame_ = true;
        }
        else
        {
            step_to_next_frame_ = false;
        }
        // --------------------------------------------------------------------

        d_cam.Activate(s_cam);
        pangolin::glDrawColouredCube();

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
