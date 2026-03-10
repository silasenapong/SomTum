#include "weeklypage.h"
#include "ui_weeklypage.h"


WeeklyPage::WeeklyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeeklyPage)
{
    ui->setupUi(this);

    showData();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &WeeklyPage::showData);
    timer->start(300000);
}

void WeeklyPage::showData(){

    data.loadData("output.txt");

    //Declare
    QString level_text;
    double graph_max = 100;

    //set dd/mm/yy to dd/mm
    data.daily.date[5]='\0';

    //dynamic graph
    if (data.monthly.max>100) graph_max = data.monthly.max;

    // show weekly_avg
    ui->weekly_avg->setText("ค่าเฉลี่ยสัปดาห์นี้: " + QString::number(data.weekly.avg) + " µg/m³" );

    // show weekly_day_max , weekly_max
    ui->weekly_day_max->setText("วันที่เกิดค่าสูงสุด: " + QString(data.weekly.max_date) + "  ( " + QString::number(data.weekly.max) + " µg/m³" + " )" );

    // show weekly_day_min , weekly_min
    ui->weekly_day_min->setText("วันที่เกิดค่าต่ำสุด: " + QString(data.weekly.min_date) + "  ( " + QString::number(data.weekly.min) + " µg/m³" + " )" );

    // show PM 2.5 level
    if (data.weekly.avg >= 75.1) level_text = "มีผลกระทบต่อสุขภาพ";
    else if (data.weekly.avg >= 37.6) level_text = "เริ่มมีผลกระทบต่อสุขภาพ";
    else if (data.weekly.avg >= 25.1) level_text = "ปานกลาง";
    else if (data.weekly.avg >= 15.1) level_text = "ดี";
    else level_text = "ดีมาก";
    ui->weekly_risk_level->setText("คุณภาพอากาศสัปดาห์นี้: " + level_text);

    //bar charts (weekly)
    QBarSeries *weekly_bar = new QBarSeries();
    QBarSet *set_1 = new QBarSet(" 1 ");

    for (int i = 0; i < 6; i++){
        set_1->append(data.weekly.past[i]); //value
    }
    set_1->append(data.daily.pm);


    set_1->setColor(Qt::blue);

    weekly_bar->append(set_1);

    QChart *chart = new QChart();
    chart->addSeries(weekly_bar);
    chart->setTitle("ค่า PM 2.5 สัปดาห์นี้");  // chart_name

    QStringList day_list;

    for (int i = 0; i < 6; i++){
        day_list.append(QString(data.weekly.past_date[5-i]));
    }
    day_list.append(QString(data.daily.date));

    QBarCategoryAxis *axis_x = new QBarCategoryAxis();  // AxisX
    axis_x->append(day_list);
    chart->addAxis(axis_x, Qt::AlignBottom);
    weekly_bar->attachAxis(axis_x);

    QValueAxis *axis_y = new QValueAxis(); // AxisY
    axis_y->setRange(0,graph_max);
    if (graph_max <= 100) axis_y->setLabelFormat("%.0f");
    chart->addAxis(axis_y, Qt::AlignLeft);
    weekly_bar->attachAxis(axis_y);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setVisible(true);

    QChartView *w_chartview = new QChartView(chart);
    w_chartview->setRenderHint(QPainter::Antialiasing);
    w_chartview->setVisible(true);

    chart->setAnimationOptions(QChart::AllAnimations);

    ui->w_barchart->layout()->addWidget(w_chartview);

}

WeeklyPage::~WeeklyPage()
{
    delete ui;
}
