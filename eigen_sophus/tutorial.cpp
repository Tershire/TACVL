// tutorial.cpp
// 2023 AUG 06
// Tershire

// referred:

// command:


#include "common_include.h"


int main(int argc, char **argv)
{
    SE3 T = SE3(SO3(), Vec3::Zero());

    Mat23 A = Mat23::Identity();

    Mat23 B = A * T.rotationMatrix();

    std::cout << B << std::endl;

    return 0;
}