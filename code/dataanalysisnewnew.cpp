#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    //รายชื่อไฟล์
    string fileNames[5] = {
        "../data/data_2026_02_06.txt",
        "../data/data_2026_02_09.txt",
        "../data/data_2026_02_10.txt",
        "../data/data_2026_02_11.txt"
    };

    string line;
    string date[5000];
    string timeData[5000];
    double temperature[5000];
    double humidity[5000];
    double pm25[5000];

    int count = 0;

    //อ่านทุกไฟล์
    for (int f = 0; f < 5; f++) {

        ifstream file(fileNames[f]);

        while (getline(file, line)) {

            int pos;

            pos = line.find(",");
            date[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            timeData[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            temperature[count] = stod(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(",");
            humidity[count] = stod(line.substr(0, pos));
            line.erase(0, pos + 1);

            pm25[count] = stod(line);

            count++;
        }

        file.close();
    }

    //DAILY
    string today = date[count - 1];

    double maxToday = -1;
    double minToday = 9999;
    string timeMax;

    for (int i = 0; i < count; i++) {

        if (date[i] == today) {

            if (pm25[i] > maxToday) {
                maxToday = pm25[i];
                timeMax = timeData[i];
            }

            if (pm25[i] < minToday) {
                minToday = pm25[i];
            }
        }
    }

    //WEEKLY
    double weeklySum = 0;
    int weeklyCount = 0;

    double maxWeek = -1;
    double minWeek = 9999;
    string maxWeekDate;
    string minWeekDate;

    for (int i = count - 1; i >= 0 && weeklyCount < 7 * 24; i--) {

        weeklySum += pm25[i];
        weeklyCount++;

        if (pm25[i] > maxWeek) {
            maxWeek = pm25[i];
            maxWeekDate = date[i];
        }

        if (pm25[i] < minWeek) {
            minWeek = pm25[i];
            minWeekDate = date[i];
        }
    }

    double avgWeek = weeklySum / weeklyCount;

    //OUTPUT
    cout << maxToday << endl;
    cout << minToday << endl;
    cout << timeMax << endl;
    cout << avgWeek << endl;
    cout << maxWeekDate << endl;
    cout << maxWeek << endl;
    cout << minWeekDate << endl;
    cout << minWeek << endl;

    return 0;
}