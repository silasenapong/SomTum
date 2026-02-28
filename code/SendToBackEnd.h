#pragma once

#include <string>
#include <vector>
#include "DataTypes.h"
using namespace std;

void sendToBackend(const string& outputPath,
                   const vector<AirRecord>& records,
                   const AnalysisResult& result);
                   
//pp