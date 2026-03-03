/*
    api from web
    https://home.openweathermap.org

    compile code :
    g++ main_SaveDataAPI.cpp -o weather.exe -lws2_32 -lcrypt32

    data format :
    YYYY/MM/DD,HH:MM:SS,TEMP,HUMI,PM2.5

*/

#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <filesystem>

using json = nlohmann::json;

std::string CurrTime();
std::string getEnvVar(std::string key);
void fetchData();

int main()
{
    while (true)
    {
        fetchData();
        // change time to sleep
        std::this_thread::sleep_for(std::chrono::minutes(5));
    }
    return 0;
}

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

    std::ostringstream oss;

    // Year/Month/Day
    oss << year << "/"
        << std::setfill('0') << std::setw(2) << month << "/"
        << std::setfill('0') << std::setw(2) << monthDay << ",";

    // Hour:Min:Sec
    oss << std::setfill('0') << std::setw(2) << hour << ":"
        << std::setfill('0') << std::setw(2) << min << ":"
        << std::setfill('0') << std::setw(2) << sec;

    std::string DateTime = oss.str();

    return DateTime;
}

std::string getEnvVar(std::string key)
{
    std::ifstream file("api.env");
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream is_line(line);
        std::string l_key;
        if (std::getline(is_line, l_key, '='))
        {
            std::string l_value;
            if (std::getline(is_line, l_value))
            {
                if (l_key == key)
                    return l_value;
            }
        }
    }
    return "";
}

void fetchData()
{

    // api key in api.env
    std::string apiKey = getEnvVar("OPENWEATHER_API_KEY");

    if (apiKey.empty())
    {
        std::cout << "Error: API Key not found in .env file" << std::endl;
        return;
    }

    // path to data folder
    std::string path = "../../data/data_api.txt";
    std::ofstream write(path, std::ios::app);

    // location
    std::string lat = "18.7883";
    std::string lon = "98.9853";

    // website
    httplib::Client cli("http://api.openweathermap.org");

    auto res_w = cli.Get(("/data/2.5/weather?lat=" + lat + "&lon=" + lon +
                          "&appid=" + apiKey + "&units=metric")
                             .c_str());

    auto res_a = cli.Get(("/data/2.5/air_pollution?lat=" + lat + "&lon=" +
                          lon + "&appid=" + apiKey)
                             .c_str());

    if (res_w && res_w->status == 200 && res_a && res_a->status == 200)
    {
        auto j_w = json::parse(res_w->body);
        auto j_a = json::parse(res_a->body);

        // terminal output
        std::cout << "\n---------------------------------" << std::endl;
        std::cout << "Temp     : " << std::fixed << std::setprecision(1) << j_w["main"]["temp"] << " C" << std::endl;
        std::cout << "Humidity : " << j_w["main"]["humidity"] << " %" << std::endl;
        std::cout << "PM 2.5   : " << j_a["list"][0]["components"]["pm2_5"] << " ug/m3" << std::endl;
        std::cout << "---------------------------------" << std::endl;

        // data logger
        write << CurrTime() << ","
              << std::fixed << std::setprecision(1) << j_w["main"]["temp"] << ","
              << j_w["main"]["humidity"] << ","
              << j_a["list"][0]["components"]["pm2_5"] << "\n";
    }
    else
    {
        std::cout << "Error: Cannot fetch data from API." << std::endl;
    }

    write.close();
}