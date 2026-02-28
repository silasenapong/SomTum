#include <iostream>
#include <fstream>
#include <sstream>
#include "ReceiveData.h"
using namespace std;

vector<AirRecord> receiveDataFromInput(const string& filePath) {

    vector<AirRecord> records;
    ifstream file(filePath);

    if (!file.is_open()) {
        cerr << "[ReceiveData] ERROR: Cannot open file: " << filePath << endl;
        return records;
    }

    string line;
    int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;


        if (line.empty()) continue;

        if (lineNum == 1 && !isdigit(line[0])) continue;

        AirRecord rec;
        stringstream ss(line);
        string token;

        try {

            getline(ss, rec.date, ',');

            getline(ss, rec.time, ',');

            getline(ss, token, ',');
            rec.temperature = stod(token);

            getline(ss, token, ',');
            rec.humidity = stod(token);

            getline(ss, token);
            rec.pm25 = stod(token);

            //PM2.5
            if (rec.pm25 < 0 || rec.pm25 > 1000) {
                cerr << "[ReceiveData] WARN: Out-of-range PM2.5 = "
                     << rec.pm25 << " at line " << lineNum << " — skipped.\n";
                continue;
            }

            //temperature
            if (rec.temperature < -50 || rec.temperature > 100) {
                cerr << "[ReceiveData] WARN: Out-of-range TEMP = "
                     << rec.temperature << " at line " << lineNum << " — skipped.\n";
                continue;
            }

            //humidity
            if (rec.humidity < 0 || rec.humidity > 100) {
                cerr << "[ReceiveData] WARN: Out-of-range HUMID = "
                     << rec.humidity << " at line " << lineNum << " — skipped.\n";
                continue;
            }

            records.push_back(rec);

        } catch (...) {
            cerr << "[ReceiveData] WARN: Could not parse line "
                 << lineNum << " — skipped.\n";
        }
    }

    file.close();

    cout << "[ReceiveData] Done — received "
         << records.size() << " valid records from: " << filePath << "\n";

    return records;
}
//pp