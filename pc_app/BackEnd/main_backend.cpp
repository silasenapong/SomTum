#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <string>
#include "../httplib.h"
#include "../json.hpp"

using json = nlohmann::json;

int main()
{
    std::string host = "icwahfhaszpsfgbrveyp.supabase.co";
    std::string anon_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6Imljd2FoZmhhc3pwc2ZnYnJ2ZXlwIiwicm9sZSI6ImFub24iLCJpYXQiOjE3NzI1MTczMTUsImV4cCI6MjA4ODA5MzMxNX0.l3InWbi3kkc0z8LnbsJsGuFlSareUCbAC_jJPxhuf3Q";

    httplib::Client cli("https://" + host);

    httplib::Headers headers = {
        {"apikey", anon_key},
        {"Authorization", "Bearer " + anon_key},
        {"Content-Type", "application/json"}};

    json newUser = {
        {"username", "cpp_user"},
        {"email", "user@example.com"}};

    auto res_post = cli.Post("/rest/v1/users", headers, newUser.dump(), "application/json");

    if (res_post && res_post->status == 201)
    {
        std::cout << "Insert successful" << std::endl;
    }

    auto res_get = cli.Get("/rest/v1/users?select=*", headers);

    if (res_get && res_get->status == 200)
    {
        json data = json::parse(res_get->body);
        std::cout << "Data: " << data.dump(4) << std::endl;
    }
    else
    {
        std::cout << "Error: " << res_get.error() << std::endl;
    }

    return 0;
}
