#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string airLevel(double value) {
    if (value <= 25) return "Good";
    else if (value <= 50) return "Normal";
    else if (value <= 100) return "Unhealthy";
    else return "Dangerous";
}
double getAverage(string targetDate,string date[],double pm25[],int count) {

    double sum = 0;
    int c = 0;

    for (int i = 0; i < count; i++) {
        if (date[i] == targetDate) {
            sum += pm25[i];
            c++;
        }
    }

    if (c == 0)
        return 0;

    return sum / c;
}

int main() {

    //เปิดไฟล์ทั้งหมด
    string filenames[5] = {
        "../data/data.txt",
        "../data/data_2026_02_06.txt",
        "../data/data_2026_02_09.txt",
        "../data/data_2026_02_10.txt",
        "../data/data_2026_02_11.txt"
    };

    string date[10000];//วันที่
    string timeData[10000];//เวลาณตอนนั้น
    double pm25[10000];//ค่าpm2.5ณตอนนั้น

    int count = 0;

    for (int f = 0; f < 5; f++) {

        ifstream file(filenames[f]);
        if (!file) continue;

        string line;

        while (getline(file, line)) {

            int pos;

            pos = line.find(",");
            if (pos == string::npos) continue;
            date[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            if (pos == string::npos) continue;
            timeData[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            if (pos == string::npos) continue;
            line.erase(0, pos + 1); // ข้าม temperature

            pos = line.find(",");
            if (pos == string::npos) continue;
            line.erase(0, pos + 1); // ข้าม humidity

            if (line == "") continue;

            pm25[count] = stod(line);

            count++;
        }

        file.close();
    }

    if (count == 0) {
        cout << "0";
        return 0;
    }

    //TODAY
    string today = date[count - 1];
    string nowTime = timeData[count - 1];
    string nowHour = nowTime.substr(0, 2);

    double currentPM = pm25[count - 1];

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

    string uniqueDate[7];
    int dateCount = 0;

    for (int i = count - 1; i >= 0 && dateCount < 7; i--) {

        bool found = false;
        for (int j = 0; j < dateCount; j++)
            if (uniqueDate[j] == date[i]) found = true;

        if (!found) {
            uniqueDate[dateCount] = date[i];
            dateCount++;
        }

        weeklySum += pm25[i];
        weeklyCount++;
    }

    double avgWeek = weeklyCount > 0 ? weeklySum / weeklyCount : 0;

    double maxWeek = -1;
    double minWeek = 9999;
    string maxWeekDate;
    string minWeekDate;

    for (int i = 0; i < count; i++) {

        for (int d = 0; d < dateCount; d++) {

            if (date[i] == uniqueDate[d]) {

                if (pm25[i] > maxWeek) {
                    maxWeek = pm25[i];
                    maxWeekDate = date[i];
                }

                if (pm25[i] < minWeek) {
                    minWeek = pm25[i];
                    minWeekDate = date[i];
                }
            }
        }
    }

    //MONTH
    string thisMonth = today.substr(0, 7);
    double monthSum = 0;
    int monthCount = 0;

    double maxMonth = -1;
    double minMonth = 9999;
    string maxMonthDate;
    string minMonthDate;

    for (int i = 0; i < count; i++) {

        if (date[i].substr(0, 7) == thisMonth) {

            monthSum += pm25[i];
            monthCount++;

            if (pm25[i] > maxMonth) {
                maxMonth = pm25[i];
                maxMonthDate = date[i];
            }

            if (pm25[i] < minMonth) {
                minMonth = pm25[i];
                minMonthDate = date[i];
            }
        }
    }

    double avgMonth = monthCount > 0 ? monthSum / monthCount : 0;

    //YEAR
    string thisYear = today.substr(0, 4);
    double yearSum = 0;
    int yearCount = 0;

    for (int i = 0; i < count; i++) {
        if (date[i].substr(0, 4) == thisYear) {
            yearSum += pm25[i];
            yearCount++;
        }
    }

    double avgYear = yearCount > 0 ? yearSum / yearCount : 0;
    //OUTPUT

    cout << today << " ";//วันที่วันนี้
    cout << nowHour << " ";//เวลาตอนนี้
    cout << currentPM << " ";//ค่า pm2.5 ตอนนี้
    cout << maxToday << " ";//ค่าสูงสุดวันนี้
    cout << minToday << " ";//ค่าต่ำสุดวันนี้
    cout << timeMax << " ";//เวลาที่เกิดค่าสูงสุด
    //ค่า pm ตอนเที่ยงคืน - เที่ยงคืนอีกวัน
    double pmDay[24];

    for (int i = 0; i < 24; i++) {
    pmDay[i] = 0;
    }

    for (int i = 0; i < count; i++) {

    if (date[i] == today) {

        int hour = stoi(timeData[i].substr(0, 2));

        pmDay[hour] = pm25[i];
    }
    }
    for (int i = 0; i < 24; i++) {
    cout << pmDay[i]<< " ";
    }
    cout << avgWeek << " ";//ค่าเฉลี่ย PM2.5 ทั้งสัปดาห์
    cout << maxWeekDate << " ";//วันที่มีค่าสูงสุดในสัปดาห์
    cout << maxWeek << " ";//ค่าของวันที่ค่าสูงสุด
    cout << minWeekDate << " ";//วันที่ค่าต่ำสุดในสัปดาห์
    cout << minWeek << " ";//ค่าของวันที่ค่าต่ำสุด
    //หา 6 วันที่ย้อนหลัง
    string day1 = "";//เมื่่อวาน
    string day2 = "";//2วันก่อน
    string day3 = "";//3วันก่อน
    string day4 = "";//4วันก่อน
    string day5 = "";//5วันก่อน
    string day6 = "";//6วันก่อน

    int found = 0;

    for (int i = count - 1; i >= 0; i--) {

        if (date[i] != today) {

            if (found == 0) {
            day1 = date[i];
            found++;
            }
            else if (found == 1 && date[i] != day1) {
            day2 = date[i];
            found++;
            }
            else if (found == 2 && date[i] != day2) {
            day3 = date[i];
            found++;
            }
            else if (found == 3 && date[i] != day3) {
            day4 = date[i];
            found++;
            }
            else if (found == 4 && date[i] != day4) {
            day5 = date[i];
            found++;
            }
            else if (found == 5 && date[i] != day5) {
            day6 = date[i];
            break;
            }
        }
    }
    cout << getAverage(day1, date, pm25, count) << " ";
    cout << getAverage(day2, date, pm25, count) << " ";
    cout << getAverage(day3, date, pm25, count) << " ";
    cout << getAverage(day4, date, pm25, count) << " ";
    cout << getAverage(day5, date, pm25, count) << " ";
    cout << getAverage(day6, date, pm25, count) << " ";
    cout << avgMonth << " ";//ค่าเฉลี่ยทั้งเดือน
    cout << maxMonthDate << " ";//วันที่ค่าสูงสุดในเดือน
    cout << maxMonth << " ";//ค่าของวันที่ค่าสูงสุด
    cout << minMonthDate << " ";//วันที่ค่าต่ำสุดในเดือน
    cout << minMonth << " ";//ค่าของวันที่ค่าต่ำสุด
    cout << avgYear;//ค่าเฉลี่ยทั้งปี

    return 0;
}