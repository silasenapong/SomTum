/*
    api from web
    https://home.openweathermap.org
*/

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <map>
#include <sstream>
#include <thread>
#include <chrono>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

std::map<std::string, std::string> loadEnv(const std::string &filename)
{
    std::map<std::string, std::string> env;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream is_line(line);
        std::string key, value;
        if (std::getline(is_line, key, '='))
        {
            if (std::getline(is_line, value))
                env[key] = value;
        }
    }
    return env;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    userp->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string callAPI(std::string url)
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            std::cerr << "\n[CURL Error] " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

int main()
{
    auto env = loadEnv(".env");
    std::string apiKey = env["OPENWEATHER_API_KEY"];
    std::string lat = env["LAT"];
    std::string lon = env["LON"];

    if (apiKey.empty())
    {
        std::cerr << "CRITICAL ERROR: API Key not found in .env file!" << std::endl;
        return 1;
    }

    std::cout << "========================================" << std::endl;
    std::cout << "   Weather Logger: Chiang Mai Edition" << std::endl;
    std::cout << "   Location: " << lat << ", " << lon << std::endl;
    std::cout << "========================================" << std::endl;

    while (true)
    {
        std::cout << "\n[Status] Fetching Chiang Mai weather..." << std::flush;

        try
        {
            std::string weatherUrl = "https://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + apiKey + "&units=metric";
            std::string response = callAPI(weatherUrl);

            if (response.empty())
                throw std::runtime_error("Empty API response");

            auto weatherData = json::parse(response);
            double temp = weatherData["main"]["temp"];
            int humid = weatherData["main"]["humidity"];

            std::time_t t = std::time(nullptr);
            std::tm *tm = std::localtime(&t);
            char timeStr[20];
            std::strftime(timeStr, sizeof(timeStr), "%Y/%m/%d %H:%M:%S", tm);

            std::cout << "\n>>> Result: " << temp << "C | " << humid << "% Humidity (" << timeStr << ")" << std::endl;

            std::ofstream outFile("data_log.txt", std::ios_base::app);
            outFile << timeStr << "," << temp << "," << humid << "\n";
            outFile.close();
        }
        catch (const std::exception &e)
        {
            std::cerr << "\n>>> Error: " << e.what() << std::endl;
        }

        std::cout << "[Waiting] Next update in 60s..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    return 0;
}