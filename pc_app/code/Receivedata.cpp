#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include "ReceiveData.h"
using namespace std;

// ── Helper: strip \r and whitespace from end of string ──
static void stripLineEnding(string& s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' '))
        s.pop_back();
}

// ── Helper: count commas to detect format ──
static int countCommas(const string& s) {
    int n = 0;
    for (char c : s) if (c == ',') n++;
    return n;
}

// ── Helper: convert "YYYY/MM/DD" → comparable integer YYYYMMDD ──
static int dateToInt(const string& date) {
    string s = date;
    s.erase(remove(s.begin(), s.end(), '/'), s.end());
    try { return stoi(s); } catch (...) { return 0; }
}

// ── Helper: convert "HH:MM:SS" → seconds since midnight ──
static int timeToSeconds(const string& t) {
    try {
        int h = stoi(t.substr(0, 2));
        int m = stoi(t.substr(3, 2));
        int s = stoi(t.substr(6, 2));
        return h * 3600 + m * 60 + s;
    } catch (...) { return 0; }
}

// ── Helper: extract hour string "HH" from "HH:MM:SS" ──
static string getHourStr(const string& t) {
    return t.substr(0, 2);      // e.g. "11:16:46" → "11"
}

// ── Helper: add one day to "YYYY/MM/DD" ──
static string nextDay(const string& date) {
    int y = stoi(date.substr(0, 4));
    int m = stoi(date.substr(5, 2));
    int d = stoi(date.substr(8, 2));

    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0)) daysInMonth[2] = 29;

    d++;
    if (d > daysInMonth[m]) { d = 1; m++; }
    if (m > 12)             { m = 1; y++; }

    return to_string(y) + "/"
         + (m < 10 ? "0" : "") + to_string(m) + "/"
         + (d < 10 ? "0" : "") + to_string(d);
}

// ─────────────────────────────────────────────
//  groupByHour
//  Takes raw per-second/per-minute records and
//  collapses them into 1 averaged record per hour.
//  Key  = "YYYY/MM/DD HH"
//  Time = "HH:00:00" (start of that hour)
// ─────────────────────────────────────────────
static vector<AirRecord> groupByHour(const vector<AirRecord>& raw) {

    // Map key → running totals
    struct HourBucket {
        string date;
        string hourStr;     // "HH"
        double sumPM25  = 0;
        double sumTemp  = 0;
        double sumHumid = 0;
        int    count    = 0;
    };

    // Use ordered map so output is sorted by date+hour
    map<string, HourBucket> buckets;

    for (const AirRecord& r : raw) {
        string hour = getHourStr(r.time);          // "11"
        string key  = r.date + " " + hour;         // "2026/02/12 11"

        HourBucket& b = buckets[key];
        b.date    = r.date;
        b.hourStr = hour;
        b.sumPM25  += r.pm25;
        b.sumTemp  += r.temperature;
        b.sumHumid += r.humidity;
        b.count++;
    }

    vector<AirRecord> hourly;
    for (auto& kv : buckets) {
        HourBucket& b = kv.second;
        AirRecord rec;
        rec.date        = b.date;
        rec.time        = b.hourStr + ":00:00";    // e.g. "11:00:00"
        rec.pm25        = b.sumPM25  / b.count;
        rec.temperature = b.sumTemp  / b.count;
        rec.humidity    = b.sumHumid / b.count;
        hourly.push_back(rec);
    }

    return hourly;
}

// ─────────────────────────────────────────────
//  receiveDataFromInput
//  Reads and validates every row in the file.
//  Auto-detects Format A (3 cols) or B (5 cols).
//  Format B is grouped into hourly averages.
// ─────────────────────────────────────────────
vector<AirRecord> receiveDataFromInput(const string& filePath) {

    vector<AirRecord> raw;
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "[ReceiveData] ERROR: Cannot open file: " << filePath << "\n";
        return raw;
    }

    string line;
    int lineNum    = 0;
    int formatCols = 0;   // 3 = Format A,  5 = Format B

    while (getline(file, line)) {
        lineNum++;
        stripLineEnding(line);
        if (line.empty()) continue;

        // ── Detect format on first non-empty line ──
        if (formatCols == 0) {
            int commas = countCommas(line);
            if      (commas == 2) formatCols = 3;
            else if (commas == 4) formatCols = 5;
            else {
                cerr << "[ReceiveData] WARN: Unrecognised format — skipping file.\n";
                break;
            }
            // Skip header row if starts with a letter
            if (!isdigit(line[0])) continue;
        }

        AirRecord rec;
        stringstream ss(line);
        string token;

        try {
            if (formatCols == 3) {
                // ── Format A: YYYY/MM/DD, HH:MM:SS, PM2.5 ──
                getline(ss, rec.date, ',');
                getline(ss, rec.time, ',');
                getline(ss, token);
                stripLineEnding(token);
                rec.pm25        = stod(token);
                rec.temperature = -1;   // not in this format
                rec.humidity    = -1;   // not in this format

            } else {
                // ── Format B: YYYY/MM/DD, HH:MM:SS, PM2.5, TEMP, HUMID ──
                getline(ss, rec.date, ',');
                getline(ss, rec.time, ',');
                getline(ss, token,    ',');  rec.pm25        = stod(token);
                getline(ss, token,    ',');  rec.temperature = stod(token);
                getline(ss, token);
                stripLineEnding(token);
                rec.humidity = stod(token);
            }

            // ── Validation ──
            if (rec.pm25 < 0 || rec.pm25 > 1000) {
                cerr << "[ReceiveData] WARN: PM2.5 out of range ("
                     << rec.pm25 << ") at line " << lineNum << " — skipped.\n";
                continue;
            }
            if (rec.temperature != -1 && (rec.temperature < -50 || rec.temperature > 100)) {
                cerr << "[ReceiveData] WARN: TEMP out of range ("
                     << rec.temperature << ") at line " << lineNum << " — skipped.\n";
                continue;
            }
            if (rec.humidity != -1 && (rec.humidity < 0 || rec.humidity > 100)) {
                cerr << "[ReceiveData] WARN: HUMID out of range ("
                     << rec.humidity << ") at line " << lineNum << " — skipped.\n";
                continue;
            }

            raw.push_back(rec);

        } catch (...) {
            cerr << "[ReceiveData] WARN: Cannot parse line "
                 << lineNum << " — skipped.\n";
        }
    }

    file.close();

    // ── Format A: already hourly, return as-is ──
    if (formatCols == 3) {
        cout << "[ReceiveData] Format A (3-col, hourly) — "
             << raw.size() << " records from: " << filePath << "\n";
        return raw;
    }

    // ── Format B: group per-second readings into hourly averages ──
    vector<AirRecord> hourly = groupByHour(raw);
    cout << "[ReceiveData] Format B (5-col, per-second) — "
         << raw.size() << " raw readings → "
         << hourly.size() << " hourly records from: " << filePath << "\n";
    return hourly;
}

// ─────────────────────────────────────────────
//  filterByTimeWindow
//  Keeps only records inside the PM report window:
//    START : todayDate  00:00:00  (midnight today)
//    END   : tomorrowDate  17:00:00  (5 PM next day)
//  Pass empty string "" to skip filtering.
// ─────────────────────────────────────────────
vector<AirRecord> filterByTimeWindow(const vector<AirRecord>& records,
                                     const string& todayDate)
{
    if (todayDate.empty()) return records;

    vector<AirRecord> filtered;
    string tomorrowDate = nextDay(todayDate);
    int todayInt        = dateToInt(todayDate);
    int tomorrowInt     = dateToInt(tomorrowDate);
    int cutoffSec       = 17 * 3600;   // 17:00:00

    for (const AirRecord& r : records) {
        int recDate = dateToInt(r.date);
        int recTime = timeToSeconds(r.time);

        bool inWindow = false;
        if      (recDate == todayInt)    inWindow = (recTime >= 0);
        else if (recDate == tomorrowInt) inWindow = (recTime <= cutoffSec);

        if (inWindow) filtered.push_back(r);
    }

    cout << "[ReceiveData] Window " << todayDate << " 00:00:00"
         << " → "  << tomorrowDate << " 17:00:00"
         << " — "  << filtered.size() << " hourly records kept.\n";

    return filtered;
}