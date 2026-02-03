#include <iostream>
#include "SerialPort.hpp"

int main()
{
    SerialPort *arduino = new SerialPort("\\\\.\\COM3");

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
            std::cout << "Arduino says: " << data << std::endl;
        }
    }
    return 0;
}

// compile code
// g++ SerialPort.cpp main_pc.cpp -o Tracker.exe