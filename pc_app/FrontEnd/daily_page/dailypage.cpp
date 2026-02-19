#include "dailypage.h"
#include "ui_dailypage.h"


DailyPage::DailyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DailyPage)
{
    ui->setupUi(this);

    // PM 2.5 level

    double pm_level = 25;
    QString warning_text;


    if (pm_level > 50) warning_text = "อันตราย";
    else warning_text = "ปกติ";

    ui->PM25_level->setText("ระดับคุณภาพอากาศ: " + warning_text);


    // line chart

    QLineSeries *daily_line = new QLineSeries();

    daily_line->append(0,3);
    daily_line->append(1,5);
    daily_line->append(2,7);
    daily_line->append(3,8);
    daily_line->append(4,1);
    daily_line->append(5,5);
    daily_line->append(6,10);
    daily_line->append(7,9);
    daily_line->append(8,4);
    daily_line->append(9,3);

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(daily_line);
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0,12);
    chart->axes(Qt::Horizontal).first()->setRange(0,11);
    chart->setVisible(true);

    QChartView *d_chartview = new QChartView(chart);
    d_chartview->setRenderHint(QPainter::Antialiasing);
    d_chartview->setVisible(true);

    ui->d_linechart->layout()->addWidget(d_chartview);


}

DailyPage::~DailyPage()
{
    delete ui;
}
