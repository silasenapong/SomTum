#pragma once

#include <string>
#include <vector>
using namespace std;

struct AirRecord {
    string date;         // YYYY/MM/DD
    string time;         // HH:MM:SS
    double temperature;  // TEMP
    double humidity;     // HUMID
    double pm25;         // PM2.5
};

struct AnalysisResult {
    double mean             = 0;
    double maxVal           = 0;
    double minVal           = 0;
    double median           = 0;
    double stdDev           = 0;
    string trend            = "Pending";
    double anomalyThreshold = 0;
    vector<int>    anomalyIndices;  
    vector<int>    aqiValues;  
    vector<string> aqiCategories; 
};
//pp