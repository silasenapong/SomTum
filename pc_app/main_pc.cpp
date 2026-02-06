#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "SerialPort.hpp"

int main()
{

    // file writer
    std::ofstream write;
    write.open("Y:\\file\\CMU\\year_2\\SomTum\\SomTum\\pc_app\\data.txt");

    // serial prot reader
    SerialPort *arduino = new SerialPort("\\\\.\\COM6");

    if (arduino->isConnected())
    {
        std::cout << "Connection Established" << std::endl;
    }
    else
    {
        std::cout << "Error in port name" << std::endl;
    }

    while (arduino->isConnected())
    {
        char data[256] = "";
        int readResult = arduino->readSerialPort(data, 255);

        if (readResult > 0)
        {
            // 1. Get the time at the moment data is received
            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::tm tm = {};

#if defined(_WIN32) || defined(_WIN64)
            localtime_s(&tm, &now_c);
#else
            localtime_r(&now_c, &tm);
#endif

            // 2. Format the time string
            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y/%m/%d,%H:%M:%S");

            // 3. Print and write to file
            std::cout << oss.str() << "," << data << std::endl;

            if (write.is_open())
            {
                write << oss.str() << "," << data << std::endl;
                write.flush(); // Ensure data is saved even if the app crashes
            }
        }
    }

    write.close();

    return 0;
}

// compile code
// g++ SerialPort.cpp main_pc.cpp -o Tracker.exe
// path Y:\file\CMU\year_2\SomTum\SomTum\pc_app

// run code
// ./Tracker.exe