#include <iostream>
#include <fstream>
#include <string>
using namespace std;


string formatDate(string ymd) {
    string y = ymd.substr(0,4);
    string m = ymd.substr(5,2);
    string d = ymd.substr(8,2);
    return d + "/" + m + "/" + y;
}

string formatDateNoYear(string ymd) {
    string m = ymd.substr(5,2);
    string d = ymd.substr(8,2);
    return d + "/" + m;
}

double getAverage(string targetDate, string date[], double pm25[], int count) {

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


//อ่านไฟล์ทั้งหมด
int loadAllData(string date[], string timeData[], double pm25[], int maxCount) {

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

    int count = 0;

    for (int f = 0; f < 20; f++) {

        ifstream file(filenames[f]);
        string line;

        while (getline(file, line)) {

            if (count >= maxCount) break;

            int pos;

            pos = line.find(",");
            if (pos == string::npos) continue;
            date[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(",");
            if (pos == string::npos) continue;
            timeData[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

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

    return count;
}

//Daily
void DailyStat(
    string date[], string timeData[], double pm25[], int count,
    string& today, string& nowHour,
    double& currentPM,
    double& maxToday, double& minToday, string& timeMax,
    double pmDay[24]
) {
    today          = date[count - 1];
    string nowTime = timeData[count - 1];
    nowHour        = nowTime.substr(0, 2);
    currentPM      = pm25[count - 1];

    maxToday = -1;
    minToday = 9999;

    for (int i = 0; i < 24; i++)
        pmDay[i] = 0;

    for (int i = 0; i < count; i++) {
        if (date[i] == today) {

            if (pm25[i] > maxToday) {
                maxToday = pm25[i];
                timeMax  = timeData[i];
            }

            if (pm25[i] < minToday)
                minToday = pm25[i];

            int hour = stoi(timeData[i].substr(0, 2));
            pmDay[hour] = pm25[i];
        }
    }
}

//Weekly
void WeeklyStat(
    string date[], string timeData[], double pm25[], int count,
    string today,
    double& avgWeek,
    double& maxWeek, string& maxWeekDate,
    double& minWeek, string& minWeekDate,
    string& day1, string& day2, string& day3,
    string& day4, string& day5, string& day6
) {
    double weeklySum = 0;
    int weeklyCount  = 0;

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

    avgWeek = weeklyCount > 0 ? weeklySum / weeklyCount : 0;

    maxWeek = -1;
    minWeek = 9999;

    for (int i = 0; i < count; i++) {
        for (int d = 0; d < dateCount; d++) {
            if (date[i] == uniqueDate[d]) {

                if (pm25[i] > maxWeek) {
                    maxWeek     = pm25[i];
                    maxWeekDate = date[i];
                }

                if (pm25[i] < minWeek) {
                    minWeek     = pm25[i];
                    minWeekDate = date[i];
                }
            }
        }
    }

    day1 = ""; day2 = ""; day3 = "";
    day4 = ""; day5 = ""; day6 = "";
    int found = 0;

    for (int i = count - 1; i >= 0; i--) {

        if (date[i] != today) {

            if (found == 0)                          { day1 = date[i]; found++; }
            else if (found == 1 && date[i] != day1) { day2 = date[i]; found++; }
            else if (found == 2 && date[i] != day2) { day3 = date[i]; found++; }
            else if (found == 3 && date[i] != day3) { day4 = date[i]; found++; }
            else if (found == 4 && date[i] != day4) { day5 = date[i]; found++; }
            else if (found == 5 && date[i] != day5) { day6 = date[i]; break;  }
        }
    }
}

//Monthly
void MonthlyStat(
    string date[], double pm25[], int count,
    string today,
    double& avgMonth,
    double& maxMonth, string& maxMonthDate,
    double& minMonth, string& minMonthDate
) {
    string thisMonth  = today.substr(0, 7);
    double monthSum   = 0;
    int    monthCount = 0;

    maxMonth = -1;
    minMonth = 9999;

    for (int i = 0; i < count; i++) {
        if (date[i].substr(0, 7) == thisMonth) {

            monthSum += pm25[i];
            monthCount++;

            if (pm25[i] > maxMonth) { maxMonth = pm25[i]; maxMonthDate = date[i]; }
            if (pm25[i] < minMonth) { minMonth = pm25[i]; minMonthDate = date[i]; }
        }
    }

    avgMonth = monthCount > 0 ? monthSum / monthCount : 0;
}

//Yearly
void YearlyStat(
    string date[], double pm25[], int count,
    string today,
    double& avgYear,
    double monthAvg[12],
    double& maxMonthAvg, string& maxMonthDateY,
    double& minMonthAvg, string& minMonthDateY,
    int& over3750Count
) {
    string thisYear  = today.substr(0, 4);
    double yearSum   = 0;
    int    yearCount = 0;

    for (int i = 0; i < count; i++) {
        if (date[i].substr(0, 4) == thisYear) {
            yearSum += pm25[i];
            yearCount++;
        }
    }

    avgYear = yearCount > 0 ? yearSum / yearCount : 0;

    double monthSumAll[12]   = {0};
    int    monthCountAll[12] = {0};

    string monthMaxDate[12], monthMinDate[12];
    double monthMaxValue[12], monthMinValue[12];

    for (int i = 0; i < 12; i++) {
        monthMaxValue[i] = -1;
        monthMinValue[i] = 9999;
    }

    for (int i = 0; i < count; i++) {

        int month = stoi(date[i].substr(5, 2));
        int index = month - 1;

        monthSumAll[index] += pm25[i];
        monthCountAll[index]++;

        if (pm25[i] > monthMaxValue[index]) { monthMaxValue[index] = pm25[i]; monthMaxDate[index] = date[i]; }
        if (pm25[i] < monthMinValue[index]) { monthMinValue[index] = pm25[i]; monthMinDate[index] = date[i]; }
    }

    for (int i = 0; i < 12; i++) {
        monthAvg[i] = monthCountAll[i] > 0 ? monthSumAll[i] / monthCountAll[i] : 0;
    }

    maxMonthAvg = -1;
    minMonthAvg = 9999;
    int maxMonthIndex = -1;
    int minMonthIndex = -1;
    over3750Count = 0;

    for (int i = 0; i < 12; i++) {

        if (monthAvg[i] > maxMonthAvg) {
            maxMonthAvg   = monthAvg[i];
            maxMonthIndex = i;
        }

        if (monthAvg[i] > 0 && monthAvg[i] < minMonthAvg) {
            minMonthAvg   = monthAvg[i];
            minMonthIndex = i;
        }

        if (monthAvg[i] > 37.50)
            over3750Count++;
    }

    if (maxMonthIndex >= 0) maxMonthDateY = formatDate(monthMaxDate[maxMonthIndex]);
    if (minMonthIndex >= 0) minMonthDateY = formatDate(monthMinDate[minMonthIndex]);
}

//Output ออกมา
void writeOutput(
    string today, string nowHour,
    double currentPM, double maxToday, double minToday, string timeMax, double pmDay[24],
    double avgWeek, double maxWeek, string maxWeekDate, double minWeek, string minWeekDate,
    string day1, string day2, string day3, string day4, string day5, string day6,
    string date[], double pm25[], int count,
    double avgMonth, double maxMonth, string maxMonthDate, double minMonth, string minMonthDate,
    double avgYear, double monthAvg[12],
    double maxMonthAvg, string maxMonthDateY,
    double minMonthAvg, string minMonthDateY,
    int over3750Count
) {
    ofstream out("output.txt");

    out << "@ DAILY" << endl;
    out << "daily_date = "     << formatDate(today) << endl;
    out << "daily_time = "     << nowHour           << " " << endl;
    out << "daily_pm = "       << currentPM         << " " << endl;
    out << "daily_max = "      << maxToday          << " " << endl;
    out << "daily_min = "      << minToday          << " " << endl;
    out << "daily_max_time = " << timeMax           << " " << endl;
    out << "daily_hourly = ";
    for (int i = 0; i < 24; i++) out << pmDay[i] << " ";
    out << endl;
    out << "\n";

    out << "@ WEEKLY" << endl;
    out << "weekly_avg = "      << avgWeek                << " " << endl;
    out << "weekly_max_date = " << formatDate(maxWeekDate) << endl;
    out << "weekly_max = "      << maxWeek                << " " << endl;
    out << "weekly_min_date = " << formatDate(minWeekDate) << endl;
    out << "weekly_min = "      << minWeek                << " " << endl;
    out << "weekly_past = ";
    out << getAverage(day1, date, pm25, count) << " ";
    out << getAverage(day2, date, pm25, count) << " ";
    out << getAverage(day3, date, pm25, count) << " ";
    out << getAverage(day4, date, pm25, count) << " ";
    out << getAverage(day5, date, pm25, count) << " ";
    out << getAverage(day6, date, pm25, count) << " " << endl;
    out << "weekly_date = ";
    out << formatDateNoYear(day1) << " ";
    out << formatDateNoYear(day2) << " ";
    out << formatDateNoYear(day3) << " ";
    out << formatDateNoYear(day4) << " ";
    out << formatDateNoYear(day5) << " ";
    out << formatDateNoYear(day6) << " " << endl;
    out << "\n";

    out << "@ MONTHLY" << endl;
    out << "monthly_avg = "      << avgMonth                << " " << endl;
    out << "monthly_max_date = " << formatDate(maxMonthDate) << " " << endl;
    out << "monthly_max = "      << maxMonth                << " " << endl;
    out << "monthly_min_date = " << formatDate(minMonthDate) << " " << endl;
    out << "monthly_min = "      << minMonth                << " " << endl;
    out << "\n";

    out << "@ YEARLY" << endl;
    out << "yearly_avg = "       << avgYear       << " " << endl;
    out << "yearly_max_month = " << maxMonthDateY << endl;
    out << "yearly_max = "       << maxMonthAvg   << " " << endl;
    out << "yearly_min_month = " << minMonthDateY << endl;
    out << "yearly min = "       << minMonthAvg   << " " << endl;
    out << "yearly_monthAvg = ";
    for (int i = 0; i < 12; i++) out << monthAvg[i] << " ";
    out << endl;
    out << "yearly_dayCount = "  << over3750Count << " " << endl;

    out.close();
}



int main() {

    string date[10000];
    string timeData[10000];
    double pm25[10000];

    int count = loadAllData(date, timeData, pm25, 10000);

    if (count == 0) {
        cout << "0";
        return 0;
    }

    // DAILY
    string today, nowHour, timeMax;
    double currentPM, maxToday, minToday;
    double pmDay[24];
    DailyStat(date, timeData, pm25, count,
                  today, nowHour, currentPM, maxToday, minToday, timeMax, pmDay);

    // WEEKLY
    double avgWeek, maxWeek, minWeek;
    string maxWeekDate, minWeekDate;
    string day1, day2, day3, day4, day5, day6;
    WeeklyStat(date, timeData, pm25, count, today,
                   avgWeek, maxWeek, maxWeekDate, minWeek, minWeekDate,
                   day1, day2, day3, day4, day5, day6);

    // MONTHLY
    double avgMonth, maxMonth, minMonth;
    string maxMonthDate, minMonthDate;
    MonthlyStat(date, pm25, count, today,
                    avgMonth, maxMonth, maxMonthDate, minMonth, minMonthDate);

    // YEARLY
    double avgYear, monthAvg[12], maxMonthAvg, minMonthAvg;
    string maxMonthDateY, minMonthDateY;
    int over3750Count;
    YearlyStat(date, pm25, count, today,
                   avgYear, monthAvg, maxMonthAvg, maxMonthDateY,
                   minMonthAvg, minMonthDateY, over3750Count);

    // OUTPUT
    writeOutput(
        today, nowHour, currentPM, maxToday, minToday, timeMax, pmDay,
        avgWeek, maxWeek, maxWeekDate, minWeek, minWeekDate,
        day1, day2, day3, day4, day5, day6,
        date, pm25, count,
        avgMonth, maxMonth, maxMonthDate, minMonth, minMonthDate,
        avgYear, monthAvg, maxMonthAvg, maxMonthDateY, minMonthAvg, minMonthDateY,
        over3750Count
    );

    return 0;
}