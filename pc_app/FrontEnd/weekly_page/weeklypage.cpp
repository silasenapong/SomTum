#include "weeklypage.h"
#include "ui_weeklypage.h"
<<<<<<< HEAD
=======
#include "dataManager/datamanager.h"
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64

WeeklyPage::WeeklyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WeeklyPage)
{
    ui->setupUi(this);

<<<<<<< HEAD
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
=======
    data.loadData("data.txt");

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

>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64

    set_1->setColor(Qt::blue);

    weekly_bar->append(set_1);

    QChart *chart = new QChart();
    chart->addSeries(weekly_bar);
<<<<<<< HEAD
    chart->setTitle("ค่า PM 2.5 ในแต่ละวัน");  // chart_name

    QStringList day_list;
    day_list.append("Mon");
    day_list.append("Tue");
    day_list.append("Wed");
    day_list.append("Thu");
    day_list.append("Fri");
    day_list.append("Sat");
    day_list.append("Sun");
=======
    chart->setTitle("ค่า PM 2.5 สัปดาห์นี้");  // chart_name

    QStringList day_list;

    for (int i = 0; i < 6; i++){
          day_list.append(QString(data.weekly.past_date[5-i]));
    }
     day_list.append(QString(data.daily.date));
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64

    QBarCategoryAxis *axis_x = new QBarCategoryAxis();  // AxisX
    axis_x->append(day_list);
    chart->addAxis(axis_x, Qt::AlignBottom);
    weekly_bar->attachAxis(axis_x);

    QValueAxis *axis_y = new QValueAxis(); // AxisY
<<<<<<< HEAD
    axis_y->setRange(0,100);
=======
    axis_y->setRange(0,graph_max);
    if (graph_max <= 100) axis_y->setLabelFormat("%.0f");
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
    chart->addAxis(axis_y, Qt::AlignLeft);
    weekly_bar->attachAxis(axis_y);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setVisible(true);

    QChartView *w_chartview = new QChartView(chart);
    w_chartview->setRenderHint(QPainter::Antialiasing);
    w_chartview->setVisible(true);

<<<<<<< HEAD
=======
    chart->setAnimationOptions(QChart::AllAnimations);

>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
    ui->w_barchart->layout()->addWidget(w_chartview);

}

WeeklyPage::~WeeklyPage()
{
    delete ui;
}
