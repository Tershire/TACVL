// basic_threading.cpp
// 2023 JUL 25
// Tershire

// referred: https://modoocode.com/269

// Basic threading in C++

// command:


#include <iostream>
#include <thread>


// PROTOTYPE ==================================================================
void function1();
void function2();
void function3();


// MAIN =======================================================================
int main(int argc, char **argv)
{
    std::thread thread1(function1);
    std::thread thread2(function2);
    std::thread thread3(function3);

    thread1.join();
    thread2.join();
    thread3.join();
}


// HELPER =====================================================================
void function1()
{
    for (int i = 0; i < 7; i++)
    {
        std::cout << "Running: Thread 1" << " (i = " << i << ")" << std::endl;
    }
}

void function2()
{
    for (int i = 0; i < 7; i++)
    {
        std::cout << "Running: Thread 2" << " (i = " << i << ")" << std::endl;
    }
}

void function3()
{
    for (int i = 0; i < 7; i++)
    {
        std::cout << "Running: Thread 3" << " (i = " << i << ")" << std::endl;
    }
}
