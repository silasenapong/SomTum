#pragma once

#include <string>
#include <vector>
#include "DataTypes.h"
using namespace std;

// ─────────────────────────────────────────────
//  ReceiveData.h
//  Reads and filters data from Data Input layer
//  Input format: YYYY/MM/DD,HH:MM:SS,TEMP,HUMID,PM2.5
// ─────────────────────────────────────────────

// Reads all valid records from file
vector<AirRecord> receiveDataFromInput(const string& filePath);

// Filters records to the window:
// midnight TODAY (00:00:00) → 17:00:00 NEXT DAY (41-hour window)
// as requested: PM report from midnight today → 5pm next day
vector<AirRecord> filterByTimeWindow(const vector<AirRecord>& records,
                                     const string& todayDate);