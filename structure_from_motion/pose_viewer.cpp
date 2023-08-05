// pose_viewer.cpp
// 2023 JUL 17
// Tershire

// referred: 

// Pangolin demonstrator with the pose of a 3D cube
// expressed in Euler attitude and in quaternion

// command: 


#include <iostream>
#include <unistd.h>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>
#include <pangolin/pangolin.h>


int main(int argc, char **argv)
{
    pangolin::CreateWindowAndBind("Viewer", 1024, 768);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // define projection and initial ModelView matrix
    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAt(0, -0.1, -1.8, 0, 0, 0, 0.0, -1.0, 0.0));

    // create interactive view in window
    pangolin::Handler3D handler(s_cam);
    pangolin::View& d_cam = pangolin::CreateDisplay()
        .SetBounds(0.0, 1.0, 0.0, 1.0, -1024.0f / 768.0f)
        .SetHandler(&handler);

    // side menu --------------------------------------------------------------
    // create a side menu
    pangolin::CreatePanel("menu")
        .SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(175));

    // toggle
    // ("menu.~", {initial value}, {checkbox: true, button: false})
    pangolin::Var<bool> toggle_step_by_step("menu.Step By Step", true, true );
    pangolin::Var<bool> button_next_frame  ("menu.Next Frame"  , true, false);
    // ------------------------------------------------------------------------

    while(!pangolin::ShouldQuit())
    {
        // clear screen and activate view to render into
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // side menu operation ------------------------------------------------
        if(toggle_step_by_step)
        {
        }

        if(pangolin::Pushed(button_next_frame))
            std::cout << "You Pushed a button!" << std::endl;

        // --------------------------------------------------------------------

        d_cam.Activate(s_cam);

        // set background color
        glClearColor(0.15F, 0.15F, 0.15F, 0.0F);

        // draw
        pangolin::glDrawColouredCube();

        // swap frames and process events
        pangolin::FinishFrame();
    }

    return 0;
}
