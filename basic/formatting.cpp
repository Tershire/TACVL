// formatting.cpp
// 2023 SEP 24
// Tershire


#include <iostream>

#include <boost/format.hpp>


int main(int argc, char **argv)
{
    boost::format format("./data/%s/%d.%s");
    std::string text = (format % "color" % 3.0 % "png").str();

    std::cout << text << std::endl;

    return 0;
}
