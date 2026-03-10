#include "dailypage.h"
#include "ui_dailypage.h"
<<<<<<< HEAD

=======
<<<<<<< HEAD

=======
#include "cstdlib"
#include "dataManager/datamanager.h"
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
>>>>>>> 3a89fb51330640c0e8ba8622c6504dd0bff91c00

DailyPage::DailyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DailyPage)
{
    ui->setupUi(this);

<<<<<<< HEAD
    showData();

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DailyPage::showData);
    timer->start(300000);

}

void DailyPage::showData(){
=======
<<<<<<< HEAD
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
=======
>>>>>>> 3a89fb51330640c0e8ba8622c6504dd0bff91c00
    // LoadData
    data.loadData("data.txt");

    // Declare
    QString level_text;
    double graph_max = 100;

    //dynamic graph
    if (data.daily.max>100) graph_max = data.daily.max;

    // show PM 2.5 value
    ui->daily_value->setText("ค่าฝุ่น PM 2.5 วันนี้: " + QString::number(data.daily.pm) + " µg/m³" );

    // show PM 2.5 level
    if (data.daily.pm >= 75.1) level_text = "มีผลกระทบต่อสุขภาพ";
    else if (data.daily.pm >= 37.6) level_text = "เริ่มมีผลกระทบต่อสุขภาพ";
    else if (data.daily.pm >= 25.1) level_text = "ปานกลาง";
    else if (data.daily.pm >= 15.1) level_text = "ดี";
    else level_text = "ดีมาก";
    ui->daily_level->setText("คุณภาพอากาศ: " + level_text);

    // show daily_max
    ui->daily_max->setText("ค่าสูงสุดวันนี้: " + QString::number(data.daily.max) + " µg/m³");

    // show daily_max_time
    ui->daily_max_time->setText("เวลาที่เกิดค่าสูงสุด: " + QString(data.daily.max_time) + " น.");

    // show daily_min
    ui->daily_min->setText("ค่าต่ำสุดวันนี้: " + QString::number(data.daily.min) + " µg/m³");

    // warning_text
    if (data.daily.pm >= 37.6){
        ui->warning_text->setText("คำเตือน: ควรใส่หน้ากากอนามัยเมื่อออกจากบ้าน!!!");
    }else ui->warning_text->setText(" ");

    // line chart
    QLineSeries *daily_line = new QLineSeries();

    for (int i = 0; i < 24; i++){
        daily_line->append(i,data.daily.hourly[i]);
    }
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(daily_line);
<<<<<<< HEAD
    chart->createDefaultAxes();
    chart->axes(Qt::Vertical).first()->setRange(0,12);
    chart->axes(Qt::Horizontal).first()->setRange(0,11);
    chart->setVisible(true);

    QChartView *d_chartview = new QChartView(chart);
    d_chartview->setRenderHint(QPainter::Antialiasing);
    d_chartview->setVisible(true);

    ui->d_linechart->layout()->addWidget(d_chartview);


=======
    chart->setVisible(true);

    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("เวลา");
    axisX->setRange(0,24);
    axisX->setLabelFormat("%.0f:00");

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("PM2.5 (µg/m³)");
    axisY->setRange(0,graph_max);
    if (graph_max <= 100) axisY->setLabelFormat("%.0f");

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    daily_line->attachAxis(axisX);
    daily_line->attachAxis(axisY);


    QChartView *d_chartview = new QChartView(chart);
    d_chartview->setRenderHint(QPainter::Antialiasing);
    d_chartview->setVisible(true);
    chart->setAnimationOptions(QChart::AllAnimations);
    ui->d_linechart->layout()->addWidget(d_chartview);

>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
}

DailyPage::~DailyPage()
{
    delete ui;
}
