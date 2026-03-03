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
    string filenames[20] = {
        "../data/air_2021_Q1.txt",
        "../data/air_2021_Q2.txt",
        "../data/air_2021_Q3.txt",
        "../data/air_2021_Q4.txt",
        "../data/air_2022_Q1.txt",
        "../data/air_2022_Q2.txt",
        "../data/air_2022_Q3.txt",
        "../data/air_2022_Q4.txt",
        "../data/air_2023_Q1.txt",
        "../data/air_2023_Q2.txt",
        "../data/air_2023_Q3.txt",
        "../data/air_2023_Q4.txt",
        "../data/air_2024_Q1.txt",
        "../data/air_2024_Q2.txt",
        "../data/air_2024_Q3.txt",
        "../data/air_2024_Q4.txt",
        "../data/air_2025_Q1.txt",
        "../data/air_2025_Q2.txt",
        "../data/air_2025_Q3.txt",
        "../data/air_2025_Q4.txt"
    };

    string date[10000];//วันที่
    string timeData[10000];//เวลาณตอนนั้น
    double pm25[10000];//ค่าpm2.5ณตอนนั้น

    int count = 0;

    for (int f = 0; f < 20; f++) {

    ifstream file(filenames[f]);

    string line;

    while (getline(file, line)) {

    if (count >= 10000) break;

    int pos;

    //DATE
    pos = line.find(",");
    if (pos == string::npos) continue;
    date[count] = line.substr(0, pos);
    line.erase(0, pos + 1);

    //TIMEDATE
    pos = line.find(",");
    if (pos == string::npos) continue;
    timeData[count] = line.substr(0, pos);
    line.erase(0, pos + 1);

    //PM2.5
    if (line.empty()) continue;

    try {
        pm25[count] = stod(line);
    } catch (...) {
        continue;
    }
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
    //MONTHLY STAT (ทั้งปี)

    double monthSumAll[12] = {0};
    int monthCountAll[12] = {0};
    double monthAvg[12] = {0};

    //เก็บข้อมูลแต่ละเดือน
    for (int i = 0; i < count; i++) {

    int month = stoi(date[i].substr(5, 2)); //เลขเดือน
    monthSumAll[month - 1] += pm25[i];
    monthCountAll[month - 1]++;
    }

    //คำนวณค่าเฉลี่ยแต่ละเดือน
    for (int i = 0; i < 12; i++) {
    if (monthCountAll[i] > 0)
        monthAvg[i] = monthSumAll[i] / monthCountAll[i];
    else
        monthAvg[i] = 0;
    }

    //หาค่าสูงสุด ต่ำสุด และจำนวนเดือนที่มีค่า Pm เกิน 37.50
    double maxMonthAvg = -1;
    double minMonthAvg = 9999;
    int maxMonthIndex = -1;
    int minMonthIndex = -1;
    int over3750Count = 0;

    for (int i = 0; i < 12; i++) {

    if (monthAvg[i] > maxMonthAvg) {
        maxMonthAvg = monthAvg[i];
        maxMonthIndex = i;
    }

    if (monthAvg[i] < minMonthAvg && monthAvg[i] > 0) {
        minMonthAvg = monthAvg[i];
        minMonthIndex = i;
    }

    if (monthAvg[i] > 37.50)
        over3750Count++;
    }
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
    ofstream out("output.txt");
    //OUTPUT
    out << "@ DAILY"<<endl;
    out << "daily_date = " << today << " " << endl;//วันที่วันนี้
    out << "daily_time = " << nowHour << " " << endl;//เวลาตอนนี้
    out << "daily_pm = " << currentPM << " " << endl;//ค่า pm2.5 ตอนนี้
    out << "daily_max = " << maxToday << " " << endl;//ค่าสูงสุดวันนี้
    out << "daily_min = " << minToday << " " << endl;//ค่าต่ำสุดวันนี้
    out << "daily_max_time = " << timeMax << " " << endl;//เวลาที่เกิดค่าสูงสุด
    out << "daily_hourly = ";
    for (int i = 0; i < 24; i++) {
    out << pmDay[i]<< " " << endl;
    }
    out << "@ WEEKLY" << endl;
    out << "weekly_avg = " << avgWeek << " " << endl;//ค่าเฉลี่ย PM2.5 ทั้งสัปดาห์
    out << "weekly_max_date = " << maxWeekDate << " " << endl;//วันที่มีค่าสูงสุดในสัปดาห์
    out << "weekly_max = " << maxWeek << " " << endl;//ค่าของวันที่ค่าสูงสุด
    out << "weekly_min_date = " << minWeekDate << " " << endl;//วันที่ค่าต่ำสุดในสัปดาห์
    out << "weekly_min = " << minWeek << " " << endl;//ค่าของวันที่ค่าต่ำสุด
    out << "weekly_past = ";
    out << getAverage(day1, date, pm25, count) << " ";//ค่าเฉลี่ยเมื่่อวาน
    out << getAverage(day2, date, pm25, count) << " ";//ค่าเฉลี่ย2วันก่อน
    out << getAverage(day3, date, pm25, count) << " ";//ค่าเฉลี่ย3วันก่อน
    out << getAverage(day4, date, pm25, count) << " ";//ค่าเฉลี่ย4วันก่อน
    out << getAverage(day5, date, pm25, count) << " ";//ค่าเฉลี่ย5วันก่อน
    out << getAverage(day6, date, pm25, count) << " " << endl;//ค่าเฉลี่ย6วันก่อน
    out << "weekly_date = ";//วันที่เมื่อวาน
    out << day1 << " ";//วันที่1วันก่อน
    out << day2 << " ";//วันที่2วันก่อน
    out << day3 << " ";//วันที่3วันก่อน
    out << day4 << " ";//วันที่4วันก่อน
    out << day5 << " ";//วันที่5วันก่อน
    out << day6 << " " << endl;//วันที่6วันก่อน
    out << "@ MONTHLY" << endl;
    out << "monthly_avg = " << avgMonth << " " << endl;//ค่าเฉลี่ยทั้งเดือน
    out << "monthly_max_date = " << maxMonthDate << " " << endl;//วันที่ค่าสูงสุดในเดือน
    out << "monthly_max = " << maxMonth << " " << endl;//ค่าของวันที่ค่าสูงสุด
    out << "monthly_min_date = " << minMonthDate << " " << endl;//วันที่ค่าต่ำสุดในเดือน
    out << "monthly_min = " << minMonth << " " << endl;//ค่าของวันที่ค่าต่ำสุด
    out << "@ YEARLY" << endl;
    out << "yearly_avg = " << avgYear << " " << endl;//ค่าเฉลี่ยทั้งปี
    out << (maxMonthIndex + 1) << " ";//เดือนที่มีค่าสูงสุด
    out << maxMonthAvg << " ";//ค่าของเดือนที่ค่าสูงสุด
    out << (minMonthIndex + 1) << " ";//เดือนที่มีค่าต่ำสุด
    out << minMonthAvg << " ";//ค่าของเดือนที่ค่าต่ำสุด
    out << over3750Count << " ";// จำนวนเดือนที่เกิน 37.50
    for (int i = 0; i < 12; i++) {
    out << monthAvg[i] << " ";// ค่าเฉลี่ยแต่ละเดือน
    }
    out.close();

    return 0;
}//d