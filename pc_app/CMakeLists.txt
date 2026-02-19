cmake_minimum_required(VERSION 3.15)
project(air-q-detect)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(OpenXLSX CONFIG REQUIRED)

include_directories(SaveDataSensor)

add_executable(AirAPI_App 
    SaveDataAPI/main_SavaData_API.cpp
)
target_link_libraries(AirAPI_App PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

add_executable(AirSensor_App 
    SaveDataSensor/main_SaveData.cpp
    SaveDataSensor/SerialPort.cpp
)
target_link_libraries(AirSensor_App PRIVATE OpenXLSX::OpenXLSX)
