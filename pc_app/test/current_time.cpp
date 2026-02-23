#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

int main()
{

    time_t now = time(nullptr);
    std::cout << now << std::endl;
    std::cout << ctime(&now) << std::endl;

    // date
    struct tm *localTime = localtime(&now);

    int year = localTime->tm_year + 1900;
    // std::cout << year << std::endl;

    int month = localTime->tm_mon + 1;
    // std::cout << month << std::endl;

    int monthDay = localTime->tm_mday;
    // std::cout << monthDay << std::endl;

    std::cout << year << "/" << month << "/" << monthDay << std::endl;

    // time
    int hour = localTime->tm_hour;
    // std::cout << hour << std::endl;

    int min = localTime->tm_min;
    // std::cout << min << std::endl;

    int sec = localTime->tm_sec;
    // std::cout << sec << std::endl;

    std::ostringstream oss;

    // Year/Month/Day
    oss << year << "/"
        << std::setfill('0') << std::setw(2) << month << "/"
        << std::setfill('0') << std::setw(2) << monthDay << ",";

    // Hour:Min:Sec
    oss << std::setfill('0') << std::setw(2) << hour << ":"
        << std::setfill('0') << std::setw(2) << min << ":"
        << std::setfill('0') << std::setw(2) << sec;

    std::string fullDateTime = oss.str();

    std::cout << fullDateTime << std::endl;

    return 0;
}
