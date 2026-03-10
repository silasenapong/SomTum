#include "datamanager.h"
#include <fstream>
#include <cstring>
#include <cstdio>

using namespace std;

void DataManager::loadData(const char* filename)
{
    ifstream file(filename);
    char line[500];

    while(file.getline(line, 500))
    {
        if(strlen(line) == 0 || line[0] == '@')
            continue;

        //DAILY

        if(strncmp(line,"daily_date =",strlen("daily_date ="))==0)
            sscanf(line,"daily_date = %s",daily.date);

        else if(strncmp(line,"daily_pm =",strlen("daily_pm ="))==0)
        {
            sscanf(line,"daily_pm = %lf",&daily.pm);
        }

        else if(strncmp(line,"daily_max =",strlen("daily_max ="))==0)
            sscanf(line,"daily_max = %lf",&daily.max);

        else if(strncmp(line,"daily_min =",strlen("daily_min ="))==0)
            sscanf(line,"daily_min = %lf",&daily.min);

        else if(strncmp(line,"daily_max_time =",strlen("daily_max_time ="))==0)
            sscanf(line,"daily_max_time = %s",daily.max_time);

        else if(strncmp(line,"daily_hourly =",strlen("daily_hourly ="))==0)
        {
            sscanf(line,
                   "daily_hourly = %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                   &daily.hourly[0],&daily.hourly[1],&daily.hourly[2],&daily.hourly[3],
                   &daily.hourly[4],&daily.hourly[5],&daily.hourly[6],&daily.hourly[7],
                   &daily.hourly[8],&daily.hourly[9],&daily.hourly[10],&daily.hourly[11],
                   &daily.hourly[12],&daily.hourly[13],&daily.hourly[14],&daily.hourly[15],
                   &daily.hourly[16],&daily.hourly[17],&daily.hourly[18],&daily.hourly[19],
                   &daily.hourly[20],&daily.hourly[21],&daily.hourly[22],&daily.hourly[23]);
        }

        //WEEKLY

        else if(strncmp(line,"weekly_avg =",strlen("weekly_avg ="))==0)
            sscanf(line,"weekly_avg = %lf",&weekly.avg);

        else if(strncmp(line,"weekly_max_date =",strlen("weekly_max_date ="))==0)
            sscanf(line,"weekly_max_date = %s",weekly.max_date);

        else if(strncmp(line,"weekly_max =",strlen("weekly_max ="))==0)
            sscanf(line,"weekly_max = %lf",&weekly.max);

        else if(strncmp(line,"weekly_min_date =",strlen("weekly_min_date ="))==0)
            sscanf(line,"weekly_min_date = %s",weekly.min_date);

        else if(strncmp(line,"weekly_min =",strlen("weekly_min ="))==0)
            sscanf(line,"weekly_min = %lf",&weekly.min);

        else if(strncmp(line,"weekly_past =",strlen("weekly_past ="))==0)
            sscanf(line,
                   "weekly_past = %lf %lf %lf %lf %lf %lf",
                   &weekly.past[0],&weekly.past[1],&weekly.past[2],
                   &weekly.past[3],&weekly.past[4],&weekly.past[5]);

        else if(strncmp(line,"weekly_date =",strlen("weekly_date ="))==0)
            sscanf(line,
                   "weekly_date = %s %s %s %s %s %s",
                   weekly.past_date[0],weekly.past_date[1],weekly.past_date[2],
                   weekly.past_date[3],weekly.past_date[4],weekly.past_date[5]);

        //MONTHLY

        else if(strncmp(line,"monthly_avg =",strlen("monthly_avg ="))==0)
            sscanf(line,"monthly_avg = %lf",&monthly.avg);

        else if(strncmp(line,"monthly_max_date =",strlen("monthly_max_date ="))==0)
            sscanf(line,"monthly_max_date = %s",monthly.max_date);

        else if(strncmp(line,"monthly_max =",strlen("monthly_max ="))==0)
            sscanf(line,"monthly_max = %lf",&monthly.max);

        else if(strncmp(line,"monthly_min_date =",strlen("monthly_min_date ="))==0)
            sscanf(line,"monthly_min_date = %s",monthly.min_date);

        else if(strncmp(line,"monthly_min =",strlen("monthly_min ="))==0)
            sscanf(line,"monthly_min = %lf",&monthly.min);

        //YEARLY

        else if(strncmp(line,"yearly_avg =",strlen("yearly_avg ="))==0)
            sscanf(line,"yearly_avg = %lf",&yearly.avg);

        else if(strncmp(line,"yearly_max_month =",strlen("yearly_max_month ="))==0)
            sscanf(line,"yearly_max_month = %d",&yearly.max_month);

        else if(strncmp(line,"yearly_max =",strlen("yearly_max ="))==0)
            sscanf(line,"yearly_max = %lf",&yearly.max);

        else if(strncmp(line,"yearly_min_month =",strlen("yearly_min_month ="))==0)
            sscanf(line,"yearly_min_month = %d",&yearly.min_month);

        else if(strncmp(line,"yearly_min =",strlen("yearly_min ="))==0)
            sscanf(line,"yearly_min = %lf",&yearly.min);

        else if(strncmp(line,"yearly_monthAvg =",strlen("yearly_monthAvg ="))==0)
            sscanf(line,
                   "yearly_monthAvg = %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf",
                   &yearly.monthAvg[0],&yearly.monthAvg[1],&yearly.monthAvg[2],
                   &yearly.monthAvg[3],&yearly.monthAvg[4],&yearly.monthAvg[5],
                   &yearly.monthAvg[6],&yearly.monthAvg[7],&yearly.monthAvg[8],
                   &yearly.monthAvg[9],&yearly.monthAvg[10],&yearly.monthAvg[11]);

        else if(strncmp(line,"yearly_dayCount =",strlen("yearly_dayCount ="))==0)
            sscanf(line,"yearly_dayCount = %d",&yearly.pm_count);
    }

    file.close();
}

dataManager::dataManager(QWidget *parent)
    : QWidget(parent)
{

}

dataManager::~dataManager() = default;
