find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenXLSX CONFIG REQUIRED)

target_link_libraries(AirAnalyzer PRIVATE 
    CURL::libcurl 
    nlohmann_json::nlohmann_json
    OpenXLSX::OpenXLSX
)