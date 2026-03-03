#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {

    ifstream file("../data/data.txt");

    string line;

    string date[2000];
    string timeData[2000];
    double temperature[2000];
    double humidity[2000];
    double pm25[2000];

    int count = 0;

    //อ่านไฟล์
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

    //WEEKLY (7 วันล่าสุด)
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
    cout << maxToday << " "; //ค่าสูงสุดวันนี้
    cout << minToday << " "; //ค่าตำสุดวันนี้
    cout << timeMax << " "; //เวลาที่เกิดค่าสูงสุด
    cout << avgWeek << " "; //ค่าเฉลี่ยทั้งสัปดาห์
    cout << maxWeekDate << " "; //วันที่ค่าสูงสุด
    cout << maxWeek << " "; //ค่าสูงสุดของวันที่ค่าสูงสุด
    cout << minWeekDate << " "; //วันที่ค่าต่ำสุด
    cout << minWeek << " "; //ค่าต่ำสุดของวันที่ค่าต่ำสุด

    return 0;
}