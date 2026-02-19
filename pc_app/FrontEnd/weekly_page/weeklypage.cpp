#include "weeklypage.h"
#include "ui_weeklypage.h"

WeeklyPage::WeeklyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeeklyPage)
{
    ui->setupUi(this);

    float mon, tue, wed, thu, fri, sat, sun;
        mon = 50;
        tue = 75;
        wed = 25;
        thu = 60;
        fri = 45;
        sat = 30;
        sun = 77.5;


    //bar charts (weekly)

    QBarSeries *weekly_bar = new QBarSeries();

    QBarSet *set_1 = new QBarSet(" 1 ");

    set_1->append(mon);  // mon  // value
    set_1->append(tue);  // tues
    set_1->append(wed);  // wed
    set_1->append(thu);  // thu
    set_1->append(fri);  // fri
    set_1->append(sat);  // sat
    set_1->append(sun);  // sun

    set_1->setColor(Qt::blue);

    weekly_bar->append(set_1);

    QChart *chart = new QChart();
    chart->addSeries(weekly_bar);
    chart->setTitle("ค่า PM 2.5 ในแต่ละวัน");  // chart_name

    QStringList day_list;
    day_list.append("Mon");
    day_list.append("Tue");
    day_list.append("Wed");
    day_list.append("Thu");
    day_list.append("Fri");
    day_list.append("Sat");
    day_list.append("Sun");

    QBarCategoryAxis *axis_x = new QBarCategoryAxis();  // AxisX
    axis_x->append(day_list);
    chart->addAxis(axis_x, Qt::AlignBottom);
    weekly_bar->attachAxis(axis_x);

    QValueAxis *axis_y = new QValueAxis(); // AxisY
    axis_y->setRange(0,100);
    chart->addAxis(axis_y, Qt::AlignLeft);
    weekly_bar->attachAxis(axis_y);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setVisible(true);

    QChartView *w_chartview = new QChartView(chart);
    w_chartview->setRenderHint(QPainter::Antialiasing);
    w_chartview->setVisible(true);

    ui->w_barchart->layout()->addWidget(w_chartview);

}

WeeklyPage::~WeeklyPage()
{
    delete ui;
}
