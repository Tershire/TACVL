// read_numerics_from_txt.cpp
// 2023 AUG 23
// Tershire

// referred: 

// command: 

#include <deque>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

std::deque<long long> load_timestamps(const std::string& timestamp_file_path);


int main(int argc, char **argv)
{
    // variable ///////////////////////////////////////////////////////////////
    std::deque<long long> timestamps;

    // main ///////////////////////////////////////////////////////////////////
    timestamps = load_timestamps("../data/123.txt");
    std::cout << "timestamp size: " << timestamps.size() << std::endl;

    return 0;
}

std::deque<long long> load_timestamps(const std::string& timestamp_file_path)
{
    std::ifstream file_stream(timestamp_file_path);

    std::deque<long long> timestamps;
    if (file_stream.is_open())
    {
        long long timestamp;
        while (file_stream >> timestamp)
        {
            std::cout << "timestamp: " << timestamp << std::endl;
            timestamps.push_back(timestamp);
        }
    }
    else
    {
        std::cout << "ERROR: can't open timestamp file" << std::endl;
    }
    file_stream.close();

    return timestamps;
}
