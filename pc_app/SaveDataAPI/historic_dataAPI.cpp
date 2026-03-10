
/*

    compile code
    g++ historic_dataAPI.cpp -o hisweather.exe -lws2_32

*/

#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

using json = nlohmann::json;

struct TimeRange
{
    std::string start;
    std::string end;
    std::string filename;
    std::string label;
};

std::atomic<bool> keep_running(true);
void show_spinner(std::string message);
std::string getEnvVar(std::string key);
std::string formatDateTime(long long timestamp);
void fetchData();

int main()
{
    fetchData();

    return 0;
}

void show_spinner(std::string message)
{
    const char spinner[] = {'|', '/', '-', '\\'};
    int i = 0;
    while (keep_running)
    {
        std::cout << "\r" << message << " " << spinner[i++ % 4] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "\r" << std::string(60, ' ') << "\r";
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

std::string formatDateTime(long long timestamp)
{
    std::time_t temp = static_cast<std::time_t>(timestamp);
    std::tm *t = std::localtime(&temp);
    char buffer[25];
    std::strftime(buffer, sizeof(buffer), "%Y/%m/%d,%H:%M:%S", t);
    return std::string(buffer);
}

void fetchData()
{
    std::string apiKey = getEnvVar("OPENWEATHER_API_KEY");
    if (apiKey.empty())
    {
        std::cerr << "Error: API Key not found in api.env" << std::endl;
        return;
    }

    std::string lat = "18.7883";
    std::string lon = "98.9853";

    std::string basePath = "../../data/";

    std::vector<TimeRange> quarters = {

        {"1609459200", "1617235199", "air_2021_Q1.txt", "Jan-Mar"},
        {"1617235200", "1625097599", "air_2021_Q2.txt", "Apr-Jun"},
        {"1625097600", "1633046399", "air_2021_Q3.txt", "Jul-Sep"},
        {"1633046400", "1640995199", "air_2021_Q4.txt", "Oct-Dec"},

        {"1640995200", "1648771199", "air_2022_Q1.txt", "Jan-Mar"},
        {"1648771200", "1656633599", "air_2022_Q2.txt", "Apr-Jun"},
        {"1656633600", "1664582399", "air_2022_Q3.txt", "Jul-Sep"},
        {"1664582400", "1672531199", "air_2022_Q4.txt", "Oct-Dec"},

        {"1672531200", "1680307199", "air_2023_Q1.txt", "Jan-Mar"},
        {"1680307200", "1688169599", "air_2023_Q2.txt", "Apr-Jun"},
        {"1688169600", "1696118399", "air_2023_Q3.txt", "Jul-Sep"},
        {"1696118400", "1704067199", "air_2023_Q4.txt", "Oct-Dec"},

        {"1704067200", "1711929599", "air_2024_Q1.txt", "Jan-Mar"},
        {"1711929600", "1719791999", "air_2024_Q2.txt", "Apr-Jun"},
        {"1719792000", "1727740799", "air_2024_Q3.txt", "Jul-Sep"},
        {"1727740800", "1735689599", "air_2024_Q4.txt", "Oct-Dec"},

        {"1735689600", "1743465599", "air_2025_Q1.txt", "Jan-Mar"},
        {"1743465600", "1751327999", "air_2025_Q2.txt", "Apr-Jun"},
        {"1751328000", "1759276799", "air_2025_Q3.txt", "Jul-Sep"},
        {"1759276800", "1767225599", "air_2025_Q4.txt", "Oct-Dec"}

    };

    httplib::Client cli("http://api.openweathermap.org");

    for (const auto &q : quarters)
    {
        keep_running = true;
        std::thread spinner_thread(show_spinner, "Fetching and saving to " + basePath + q.filename);

        std::string apiPath = "/data/2.5/air_pollution/history?lat=" + lat +
                              "&lon=" + lon +
                              "&start=" + q.start +
                              "&end=" + q.end +
                              "&appid=" + apiKey;

        auto res = cli.Get(apiPath.c_str());

        keep_running = false;
        spinner_thread.join();

        if (res && res->status == 200)
        {
            auto data = json::parse(res->body);

            std::string fullPath = basePath + q.filename;
            std::ofstream outFile(fullPath);

            if (outFile.is_open())
            {
                outFile << "YYYY/MM/DD,HH:MM:SS,PM2.5" << std::endl;
                for (const auto &item : data["list"])
                {
                    long long dt = item["dt"];
                    double pm25 = item["components"]["pm2_5"];
                    outFile << formatDateTime(dt) << "," << pm25 << std::endl;
                }
                outFile.close();
                std::cout << "Successfully saved to: " << fullPath << std::endl;
            }
            else
            {
                std::cerr << "Error: Could not open path " << fullPath << ". Please ensure the directory exists." << std::endl;
            }
        }
        else
        {
            std::cerr << "Failed " << q.label << " Status: " << (res ? std::to_string(res->status) : "Error") << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}