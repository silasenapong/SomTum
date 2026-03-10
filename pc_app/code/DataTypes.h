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

struct AnalysisReport {

    // ── TODAY ──────────────────────────────────
    string todayDate        = "";       // DD/MM/YYYY  e.g. 01/03/2026
    int    currentHour      = 0;        // hour only   e.g. 12:08:55 → 12
    double currentPM25      = 0;        // PM2.5 right now
    double todayMax         = 0;        // highest PM2.5 today
    double todayMin         = 0;        // lowest PM2.5 today
    string timeOfTodayMax   = "";       // HH:MM:SS when today's max occurred

    // ── HOURLY (midnight → next midnight, 24 values) ──
    // index 0 = hour 00, index 23 = hour 23
    // Hours after currentHour are sent as 0 (per PDF spec)
    double hourlyPM25[24]   = {};       // zero-initialised

    // ── WEEKLY ─────────────────────────────────
    double weeklyAvg        = 0;        // avg of today + past 6 days
    string weekMaxDate      = "";       // DD/MM  e.g. 27/02
    double weekMaxVal       = 0;
    string weekMinDate      = "";       // DD/MM  e.g. 25/02
    double weekMinVal       = 0;

    // Daily averages for past 6 days (index 0 = yesterday)
    double dailyAvg[6]      = {};       // [0]=yesterday ... [5]=6 days ago
    string dailyDate[6];                // [0]="28/02" ...

    // ── MONTHLY ────────────────────────────────
    double monthlyAvg       = 0;        // avg for current month
    string monthMaxDate     = "";       // DD/MM/YYYY
    double monthMaxVal      = 0;
    string monthMinDate     = "";       // DD/MM/YYYY
    double monthMinVal      = 0;

    // ── YEARLY ─────────────────────────────────
    double yearlyAvg             = 0;
    int    yearMaxMonth          = 0;   // month number e.g. 11
    double yearMaxMonthVal       = 0;
    int    yearMinMonth          = 0;   // month number e.g. 7
    double yearMinMonthVal       = 0;
    int    monthsExceedingLimit  = 0;   // months where avg PM2.5 > 37.50

    // Monthly averages Jan–Dec (index 0=Jan, index 11=Dec)
    // Months after current month are 0 (per PDF spec)
    double monthlyAvgPerMonth[12] = {}; // zero-initialised
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