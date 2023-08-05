// txt_IO.cpp
// 2023 AUG 04
// Tershire

// referred: https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=tkdldjs35&logNo=221308244184l


#include <iostream>
#include <cmath> // isinf(), isnan()

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

// txt
#include <fstream>


typedef Eigen::Vector3d Vec3;

int main(int argc, char **argv)
{
    const double ZERO = 0.0;

    double posinf =  1.0 / ZERO;
    double neginf = -1.0 / ZERO;
    double nan    = ZERO / ZERO;

    double num = 3.2;

    //
    std::cout << "posinf: " << posinf << std::endl
              << "isinf(posinf): " << std::isinf(posinf) << std::endl
              << "isnan(posinf): " << std::isnan(posinf) << std::endl;

    std::cout << "neginf: " << neginf << std::endl
              << "isinf(neginf): " << std::isinf(neginf) << std::endl
              << "isnan(neginf): " << std::isnan(neginf) << std::endl;

    std::cout << "nan   : " << nan    << std::endl
              << "isinf(nan)   : " << std::isinf(nan)    << std::endl
              << "isnan(nan)   : " << std::isnan(nan)    << std::endl;

    // Eigen //////////////////////////////////////////////////////////////////
    Vec3 vector = Vec3(posinf, neginf, nan);

    std::cout << "vector.hasNaN(): " << vector.hasNaN() << std::endl;

    // txt output /////////////////////////////////////////////////////////////
    std::ofstream fout;

    // open file
    fout.open("txt_IO.txt");

    // write
    fout << posinf << "\n"
         << neginf << "\n"
         << nan    << "\n"
         << num    << std::endl;

    // close file
    fout.close();

    return 0;
}