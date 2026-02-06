#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include "SerialPort.hpp"

std::string getDateString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm = {};

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm, &now_c); // window
#else
    localtime_r(&now_c, &tm); // linux or mac
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y_%m_%d");
    return oss.str();
}

int main()
{
    std::string folderPath = "Y:\\file\\CMU\\year_2\\SomTum\\SomTum\\data\\";
    std::string currentDay = "";
    std::ofstream write;

    SerialPort *arduino = new SerialPort("\\\\.\\COM6");

    if (arduino->isConnected())
    {
        std::cout << "Connection Established" << std::endl;
    }
    else
    {
        std::cerr << "Error in port name" << std::endl;
        return 1;
    }

    while (arduino->isConnected())
    {
        char data[256] = "";
        int readResult = arduino->readSerialPort(data, 255);

        if (readResult > 0)
        {
            std::string today = getDateString();

            if (today != currentDay)
            {
                if (write.is_open())
                {
                    write.close();
                }
                currentDay = today;
                std::string fullPath = folderPath + "data_" + currentDay + ".txt";

                write.open(fullPath, std::ios::app);
                std::cout << ">>> Switched to new file: " << fullPath << std::endl;
            }

            auto now = std::chrono::system_clock::now();
            std::time_t now_c = std::chrono::system_clock::to_time_t(now);
            std::tm tm = {};

#if defined(_WIN32) || defined(_WIN64)
            localtime_s(&tm, &now_c);
#else
            localtime_r(&now_c, &tm);
#endif

            std::ostringstream timeStr;
            timeStr << std::put_time(&tm, "%Y/%m/%d,%H:%M:%S");

            std::cout << timeStr.str() << "," << data << std::endl;

            if (write.is_open())
            {
                write << timeStr.str() << "," << data << std::endl;
                write.flush();
            }
        }
    }

    if (write.is_open())
        write.close();

    delete arduino;

    return 0;
}

/*
    compile code
    g++ SerialPort.cpp main_pc.cpp -o Tracker.exe

    run command
    ./Tracker.exe
*/
