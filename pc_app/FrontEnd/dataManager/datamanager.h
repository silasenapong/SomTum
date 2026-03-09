#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QWidget>


class dataManager : public QWidget
{
    Q_OBJECT

public:
    explicit dataManager(QWidget *parent = nullptr);
    ~dataManager();

};

struct DailyData{
    char date[12];
    int time;
    double pm;
    double max;
    double min;
    char max_time[10];
    double hourly[24];
};

struct WeeklyData{
    double avg;
    char max_date[12];
    double max;
    char min_date[12];
    double min;
    double past[6];
    char past_date[6][12];
};

struct MonthlyData{
    double avg;
    char max_date[12];
    double max;
    char min_date[12];
    double min;
};

struct YearlyData{
    double avg;
    int max_month;
    double max;
    int min_month;
    double min;
    double monthAvg[12];
    int pm_count;
};

class DataManager{
public:
    DailyData daily;
    WeeklyData weekly;
    MonthlyData monthly;
    YearlyData yearly;
    void loadData(const char* filename);
};

#endif
