#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <map>
#include <numeric>
#include "ReceiveData.h"
#include "SendToBackEnd.h"
using namespace std;

// ─────────────────────────────────────────────
//  main.cpp  —  Data Analysis Layer entry point
//
//  Input format : YYYY/MM/DD,HH:MM:SS,PM2.5
//             or: YYYY/MM/DD,HH:MM:SS,PM2.5,TEMP,HUMID
//  Report window: midnight TODAY → 17:00:00 NEXT DAY
//
//  Usage (with arguments):
//    ./app <inputFile> <outputFile> <todayDate>
//    ./app data/air_2021_Q1.txt output.json 2021/01/15
//
//  Usage (no arguments — program will ask you):
//    ./app
// ─────────────────────────────────────────────

// ── Helper: check if a file actually exists ──
static bool fileExists(const string& path) {
    ifstream f(path);
    return f.good();
}

// ── Helper: extract "DD/MM/YYYY" from "YYYY/MM/DD" ──
static string toDMY(const string& ymd) {
    if (ymd.size() < 10) return ymd;
    return ymd.substr(8, 2) + "/" + ymd.substr(5, 2) + "/" + ymd.substr(0, 4);
}

// ── Helper: extract "DD/MM" from "YYYY/MM/DD" ──
static string toDM(const string& ymd) {
    if (ymd.size() < 10) return ymd;
    return ymd.substr(8, 2) + "/" + ymd.substr(5, 2);
}

// ── Helper: extract hour number from "HH:MM:SS" ──
static int getHour(const string& t) {
    try { return stoi(t.substr(0, 2)); } catch (...) { return 0; }
}

// ── Helper: get "YYYY/MM/DD" date N days before a given date ──
static string daysAgo(const string& date, int n) {
    int y = stoi(date.substr(0, 4));
    int m = stoi(date.substr(5, 2));
    int d = stoi(date.substr(8, 2));

    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) daysInMonth[2] = 29;

    for (int i = 0; i < n; i++) {
        d--;
        if (d < 1) { m--; if (m < 1) { m = 12; y--; } d = daysInMonth[m]; }
    }
    return to_string(y) + "/"
         + (m < 10 ? "0" : "") + to_string(m) + "/"
         + (d < 10 ? "0" : "") + to_string(d);
}

// ─────────────────────────────────────────────
//  buildReport
//  Fills AnalysisReport from allRecords + todayDate.
//  Called while waiting for teammates' modules.
// ─────────────────────────────────────────────
static AnalysisReport buildReport(const vector<AirRecord>& allRecords,
                                  const vector<AirRecord>& windowRecords,
                                  const string& todayDate)
{
    AnalysisReport report;

    if (allRecords.empty() || windowRecords.empty()) return report;

    // ── TODAY ─────────────────────────────────
    // Collect only today's records from window
    vector<AirRecord> todayRecs;
    for (auto& r : windowRecords)
        if (r.date == todayDate) todayRecs.push_back(r);

    if (!todayRecs.empty()) {
        // Current = last record of today
        AirRecord& latest = todayRecs.back();
        report.todayDate   = toDMY(todayDate);
        report.currentHour = getHour(latest.time);
        report.currentPM25 = latest.pm25;

        // Max and min today
        auto maxIt = max_element(todayRecs.begin(), todayRecs.end(),
                     [](const AirRecord& a, const AirRecord& b){ return a.pm25 < b.pm25; });
        auto minIt = min_element(todayRecs.begin(), todayRecs.end(),
                     [](const AirRecord& a, const AirRecord& b){ return a.pm25 < b.pm25; });

        report.todayMax       = maxIt->pm25;
        report.todayMin       = minIt->pm25;
        report.timeOfTodayMax = maxIt->time;
    }

    // ── HOURLY (24 values, 0 after current hour) ──
    for (auto& r : todayRecs) {
        int h = getHour(r.time);
        if (h >= 0 && h < 24)
            report.hourlyPM25[h] = r.pm25;
    }

    // ── WEEKLY ────────────────────────────────
    // Collect today + past 6 days from allRecords
    double weekSum = 0;
    int    weekCount = 0;
    double weekMax = -1, weekMin = 1e9;
    string weekMaxDate, weekMinDate;

    for (int i = 0; i <= 6; i++) {
        string day = daysAgo(todayDate, i);
        double daySum = 0; int dayCount = 0;
        for (auto& r : allRecords) {
            if (r.date == day) { daySum += r.pm25; dayCount++; weekSum += r.pm25; weekCount++; }
        }
        if (dayCount > 0) {
            double dayAvg = daySum / dayCount;
            if (dayAvg > weekMax) { weekMax = dayAvg; weekMaxDate = day; }
            if (dayAvg < weekMin) { weekMin = dayAvg; weekMinDate = day; }

            if (i >= 1 && i <= 6) {
                report.dailyAvg[i-1]  = dayAvg;
                report.dailyDate[i-1] = toDM(day);
            }
        }
    }
    report.weeklyAvg  = (weekCount > 0) ? weekSum / weekCount : 0;
    report.weekMaxDate = toDM(weekMaxDate);
    report.weekMaxVal  = weekMax > 0 ? weekMax : 0;
    report.weekMinDate = toDM(weekMinDate);
    report.weekMinVal  = weekMin < 1e9 ? weekMin : 0;

    // ── MONTHLY ───────────────────────────────
    int currentMonth = stoi(todayDate.substr(5, 2));
    int currentYear  = stoi(todayDate.substr(0, 4));

    double monthSum = 0; int monthCount = 0;
    double monthMax = -1, monthMin = 1e9;
    string monthMaxDate, monthMinDate;

    // Group by day within this month
    map<string, pair<double,int>> dayBuckets;
    for (auto& r : allRecords) {
        int ry = stoi(r.date.substr(0, 4));
        int rm = stoi(r.date.substr(5, 2));
        if (ry == currentYear && rm == currentMonth) {
            dayBuckets[r.date].first  += r.pm25;
            dayBuckets[r.date].second += 1;
            monthSum += r.pm25;
            monthCount++;
        }
    }
    for (auto& kv : dayBuckets) {
        double dayAvg = kv.second.first / kv.second.second;
        if (dayAvg > monthMax) { monthMax = dayAvg; monthMaxDate = kv.first; }
        if (dayAvg < monthMin) { monthMin = dayAvg; monthMinDate = kv.first; }
    }
    report.monthlyAvg  = (monthCount > 0) ? monthSum / monthCount : 0;
    report.monthMaxDate = toDMY(monthMaxDate);
    report.monthMaxVal  = monthMax > 0 ? monthMax : 0;
    report.monthMinDate = toDMY(monthMinDate);
    report.monthMinVal  = monthMin < 1e9 ? monthMin : 0;

    // ── YEARLY ────────────────────────────────
    double yearSum = 0; int yearCount = 0;
    double monthAvgs[12] = {};
    int    monthCounts[12] = {};

    for (auto& r : allRecords) {
        int ry = stoi(r.date.substr(0, 4));
        int rm = stoi(r.date.substr(5, 2));
        if (ry == currentYear) {
            yearSum += r.pm25; yearCount++;
            monthAvgs[rm-1]   += r.pm25;
            monthCounts[rm-1] += 1;
        }
    }

    report.yearlyAvg = (yearCount > 0) ? yearSum / yearCount : 0;

    double bestMonthMax = -1, bestMonthMin = 1e9;
    int    monthsOver = 0;

    for (int i = 0; i < 12; i++) {
        if (monthCounts[i] > 0) {
            double avg = monthAvgs[i] / monthCounts[i];
            report.monthlyAvgPerMonth[i] = avg;
            if (avg > bestMonthMax) { bestMonthMax = avg; report.yearMaxMonth = i+1; report.yearMaxMonthVal = avg; }
            if (avg < bestMonthMin) { bestMonthMin = avg; report.yearMinMonth = i+1; report.yearMinMonthVal = avg; }
            if (avg > 37.5) monthsOver++;
        }
    }
    report.monthsExceedingLimit = monthsOver;

    return report;
}

// ─────────────────────────────────────────────
//  printSummary — shows results in the terminal
// ─────────────────────────────────────────────
static void printSummary(const AnalysisReport& r) {
    cout << "\n========================================\n";
    cout << "  PM2.5 Analysis Summary\n";
    cout << "========================================\n";
    cout << fixed << setprecision(2);

    cout << "\n[ TODAY — " << r.todayDate << " ]\n";
    cout << "  Current Hour  : " << r.currentHour   << ":00\n";
    cout << "  Current PM2.5 : " << r.currentPM25   << "\n";
    cout << "  Max PM2.5     : " << r.todayMax       << "  at " << r.timeOfTodayMax << "\n";
    cout << "  Min PM2.5     : " << r.todayMin       << "\n";

    cout << "\n[ HOURLY PM2.5 ]\n";
    for (int i = 0; i < 24; i++) {
        if (r.hourlyPM25[i] > 0)
            cout << "  " << (i < 10 ? "0" : "") << i << ":00  →  " << r.hourlyPM25[i] << "\n";
    }

    cout << "\n[ WEEKLY ]\n";
    cout << "  Weekly Avg    : " << r.weeklyAvg  << "\n";
    cout << "  Week Max      : " << r.weekMaxVal << "  on " << r.weekMaxDate << "\n";
    cout << "  Week Min      : " << r.weekMinVal << "  on " << r.weekMinDate << "\n";
    cout << "  Daily Averages (past 6 days):\n";
    for (int i = 0; i < 6; i++) {
        if (!r.dailyDate[i].empty())
            cout << "    " << r.dailyDate[i] << "  →  " << r.dailyAvg[i] << "\n";
    }

    cout << "\n[ MONTHLY ]\n";
    cout << "  Monthly Avg   : " << r.monthlyAvg  << "\n";
    cout << "  Month Max     : " << r.monthMaxVal << "  on " << r.monthMaxDate << "\n";
    cout << "  Month Min     : " << r.monthMinVal << "  on " << r.monthMinDate << "\n";

    cout << "\n[ YEARLY ]\n";
    cout << "  Yearly Avg    : " << r.yearlyAvg        << "\n";
    cout << "  Worst Month   : Month " << r.yearMaxMonth << "  →  " << r.yearMaxMonthVal << "\n";
    cout << "  Best Month    : Month " << r.yearMinMonth << "  →  " << r.yearMinMonthVal << "\n";
    cout << "  Months > 37.5 : " << r.monthsExceedingLimit << " month(s)\n";
    cout << "  Monthly Avgs  : \n";
    for (int i = 0; i < 12; i++) {
        if (r.monthlyAvgPerMonth[i] > 0)
            cout << "    Month " << (i+1 < 10 ? "0" : "") << (i+1)
                 << "  →  " << r.monthlyAvgPerMonth[i] << "\n";
    }
    cout << "\n";
}

int main(int argc, char* argv[]) {

    string inputFile;
    string outputFile;
    string todayDate;

    cout << "========================================\n";
    cout << "  Data Analysis Module\n";
    cout << "========================================\n\n";

    if (argc >= 4) {
        inputFile  = argv[1];
        outputFile = argv[2];
        todayDate  = argv[3];
    } else {
        cout << "Enter input file path  (e.g. data/air_2021_Q1.txt) : ";
        cin >> inputFile;
        cout << "Enter output file path (e.g. output.json)          : ";
        cin >> outputFile;
        cout << "Enter today's date     (YYYY/MM/DD e.g. 2026/03/01): ";
        cin >> todayDate;
        cout << "\n";
    }

    if (!fileExists(inputFile)) {
        cerr << "[main] ERROR: File not found: " << inputFile << "\n";
        cerr << "[main] Make sure the file path is correct and try again.\n";
        return 1;
    }

    cout << "  Input  : " << inputFile  << "\n";
    cout << "  Output : " << outputFile << "\n";
    cout << "  Window : " << todayDate  << " 00:00:00 → next day 17:00:00\n\n";

    // ── STEP 1 : YOUR PART ───────────────────
    cout << "[1] Receiving data from Data Input...\n";
    vector<AirRecord> allRecords = receiveDataFromInput(inputFile);

    if (allRecords.empty()) {
        cerr << "[main] No valid data received. Exiting.\n";
        return 1;
    }

    // ── STEP 2 : YOUR PART ───────────────────
    cout << "[2] Filtering to report time window...\n";
    vector<AirRecord> windowRecords = filterByTimeWindow(allRecords, todayDate);

    if (windowRecords.empty()) {
        cerr << "[main] No records found in the time window: "
             << todayDate << " 00:00:00 → next day 17:00:00\n";
        cerr << "[main] Check that your data file contains dates around: "
             << todayDate << "\n";
        return 1;
    }

    // ── STEP 3 : TEAMMATES' PART ─────────────
    // TODO: replace buildReport() with teammates' modules once ready
    //   AnalysisReport report = buildReport(windowRecords, allRecords, todayDate);
    cout << "[3] Running analysis...\n";
    AnalysisReport report = buildReport(allRecords, windowRecords, todayDate);

    // Print summary to terminal
    printSummary(report);

    // ── STEP 4 : YOUR PART ───────────────────
    cout << "[4] Sending analysis report to Backend...\n";
    sendToBackend(outputFile, report);

    cout << "[main] All steps complete.\n";
    return 0;
}