#include <iostream>
#include <ctime>

std::string CurrTime()
{
    time_t now = time(nullptr);

    // date
    struct tm *localTime = localtime(&now);

    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int monthDay = localTime->tm_mday;

    // time
    int hour = localTime->tm_hour;
    int min = localTime->tm_min;
    int sec = localTime->tm_sec;

    // std::cout << year << "/" << month << "/" << monthDay << ","
    // << hour << ":" << min << ":" << sec << std::endl;

    return std::to_string(year) + "/" + std::to_string(month) + "/" +
           std::to_string(monthDay) + "," + std::to_string(hour) + ":" +
           std::to_string(min) + ":" + std::to_string(sec);
}

int main()
{

    std::cout << CurrTime();

    return 0;
}