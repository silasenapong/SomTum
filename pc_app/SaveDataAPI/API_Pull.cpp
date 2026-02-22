#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

// ชี้พาธไปยังโฟลเดอร์ lib_external ที่เราสร้าง
// #include "curl_dist/include/curl/curl.h"
#include "curl.h"
#include "json.hpp"

using json = nlohmann::json;

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    size_t newLength = size * nmemb;
    s->append((char *)contents, newLength);
    return newLength;
}

void fetchData()
{
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl)
    {
        // เปลี่ยน URL และ API Key ของคุณ
        curl_easy_setopt(curl, CURLOPT_URL, "https://airapi.airly.eu/v2/installations/204");
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "apikey: YOUR_API_KEY_HERE");

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK)
        {
            auto j = json::parse(readBuffer);
            std::ofstream file("airly_data.txt", std::ios::app);
            file << "Logged at: " << __DATE__ << " " << __TIME__ << "\n";
            file << j.dump(4) << "\n\n";
            file.close();
            std::cout << "Success: Data saved to airly_data.txt" << std::endl;
        }
        else
        {
            std::cerr << "CURL Error: " << curl_easy_strerror(res) << std::endl;
        }
        curl_easy_cleanup(curl);
    }
}

int main()
{
    while (true)
    {
        fetchData();
        std::cout << "Sleeping for 1 hour..." << std::endl;
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
    return 0;
}