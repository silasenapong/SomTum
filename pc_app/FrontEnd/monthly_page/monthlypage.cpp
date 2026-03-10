#include "monthlypage.h"
#include "ui_monthlypage.h"

MonthlyPage::MonthlyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MonthlyPage)
{
    ui->setupUi(this);

    showData();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MonthlyPage::showData);
    timer->start(300000);

}

void MonthlyPage::showData(){
    //LoadData
    data.loadData("data.txt");

    //Declare
    QString level_text;

    // show monthly_avg
    ui->monthly_avg->setText("ค่าเฉลี่ยเดือนนี้: " + QString::number(data.monthly.avg) + " µg/m³" );

    // show monthly_day_max , monthly_max
    ui->monthly_day_max->setText("วันที่เกิดค่าสูงสุด: " + QString(data.monthly.max_date) + "  ( " + QString::number(data.monthly.max) + " µg/m³" + " )" );

    // show monthly_day_min , monthly_min
    ui->monthly_day_min->setText("วันที่เกิดค่าต่ำสุด: " + QString(data.monthly.min_date) + "  ( " + QString::number(data.monthly.min) + " µg/m³" + " )" );

    // show PM 2.5 level
    if (data.monthly.avg >= 75.1) level_text = "มีผลกระทบต่อสุขภาพ";
    else if (data.monthly.avg >= 37.6) level_text = "เริ่มมีผลกระทบต่อสุขภาพ";
    else if (data.monthly.avg >= 25.1) level_text = "ปานกลาง";
    else if (data.monthly.avg >= 15.1) level_text = "ดี";
    else level_text = "ดีมาก";
    ui->monthly_risk_level->setText("คุณภาพอากาศเดือนนี้: " + level_text);
}

MonthlyPage::~MonthlyPage()
{
    delete ui;
}
