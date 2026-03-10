#include "yearlypage.h"
#include "ui_yearlypage.h"

YearlyPage::YearlyPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::YearlyPage)
{
    ui->setupUi(this);


    showData();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &YearlyPage::showData);
    timer->start(300000);
}

void YearlyPage::showData(){

    //LoadData
    data.loadData("output.txt");

    //Declare

    QString yearly_month_max, yearly_month_min, level_text;
    double graph_max = 100;
    QString month[] = {"มกราคม","กุมภาพันธ์","มีนาคม","เมษายน","พฤษภาคม","มิถุนายน","กรกฎาคม","สิงหาคม","กันยายน","ตุลาคม","พฤศจิกายน","ธันวาคม"};

    //dynamic graph
    if (data.yearly.max>100) graph_max = data.yearly.max;

    // set
    yearly_month_max = month[data.yearly.max_month-1];
    yearly_month_min = month[data.yearly.min_month-1];


    // show yearly_avg
    ui->yearly_avg->setText("ค่าเฉลี่ยปีนี้นี้: " + QString::number(data.yearly.avg) + " µg/m³" );

    // show yearly_day_max , yearly_max
    ui->yearly_month_max->setText("เดือนที่แย่ที่สุด: " + yearly_month_max + "  ( " + QString::number(data.yearly.max) + " µg/m³" + " )" );

    // show yearly_day_min , yearly_min
    ui->yearly_month_min->setText("เดือนที่ดีที่สุด: " + yearly_month_min + "  ( " + QString::number(data.yearly.min) + " µg/m³" + " )" );

    // show PM 2.5 level
    if (data.yearly.avg >= 75.1) level_text = "มีผลกระทบต่อสุขภาพ";
    else if (data.yearly.avg >= 37.6) level_text = "เริ่มมีผลกระทบต่อสุขภาพ";
    else if (data.yearly.avg >= 25.1) level_text = "ปานกลาง";
    else if (data.yearly.avg >= 15.1) level_text = "ดี";
    else level_text = "ดีมาก";
    ui->yearly_risk_level->setText("คุณภาพอากาศทั้งปี: " + level_text);

    // show poll_day

    ui->yearly_pollday->setText("จำนวนวันที่เกินค่ามาตราฐาน: " + QString::number(data.yearly.pm_count) + " วัน" );


    //bar charts (weekly)

    QBarSeries *yearly_bar = new QBarSeries();
    QBarSet *set_1 = new QBarSet("");

    for (int i = 0; i < 12; i++){
        set_1->append(data.yearly.monthAvg[i]);
    }

    set_1->setColor(Qt::blue);

    yearly_bar->append(set_1);

    QChart *y_chart = new QChart();
    y_chart->addSeries(yearly_bar);
    y_chart->setTitle("ค่า PM 2.5 ในแต่ละเดือน");  // chart_name

    QStringList month_list;
    month_list.append("Jan");
    month_list.append("Feb");
    month_list.append("Mar");
    month_list.append("April");
    month_list.append("May");
    month_list.append("June");
    month_list.append("Jul");
    month_list.append("Aug");
    month_list.append("Oct");
    month_list.append("Sep");
    month_list.append("Nov");
    month_list.append("Dec");

    QBarCategoryAxis *axis_x = new QBarCategoryAxis();  // AxisX
    axis_x->append(month_list);
    y_chart->addAxis(axis_x, Qt::AlignBottom);
    yearly_bar->attachAxis(axis_x);

    QValueAxis *axis_y = new QValueAxis(); // AxisY
    axis_y->setRange(0,graph_max);
    y_chart->addAxis(axis_y, Qt::AlignLeft);
    yearly_bar->attachAxis(axis_y);

    y_chart->legend()->setVisible(true);
    y_chart->legend()->setAlignment(Qt::AlignBottom);
    y_chart->setVisible(true);

    QChartView *y_barchart = new QChartView(y_chart);
    y_barchart->setRenderHint(QPainter::Antialiasing);
    y_barchart->setVisible(true);

    y_chart->setAnimationOptions(QChart::AllAnimations);

    // line chart
    QLineSeries *yearly_line = new QLineSeries();

    for (int i = 0; i < 12; i++){
        yearly_line->append(i+1,data.yearly.monthAvg[i]);
    }

    QChart *chart = new QChart();
    chart->legend()->hide();
    chart->addSeries(yearly_line);
    chart->setVisible(true);

    QCategoryAxis *axis_X = new QCategoryAxis();

    axis_X->append("Jan",1.5);
    axis_X->append("Feb",2.5);
    axis_X->append("Mar",3.5);
    axis_X->append("Apr",4.5);
    axis_X->append("May",5.5);
    axis_X->append("Jun",6.5);
    axis_X->append("Jul",7.5);
    axis_X->append("Aug",8.5);
    axis_X->append("Sep",9.5);
    axis_X->append("Oct",10.5);
    axis_X->append("Nov",11.5);
    axis_X->append("Dec",12.5);

    axis_X->setRange(1,13);
    axis_X->setTitleText("เดือน");

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("PM2.5 (µg/m³)");
    axisY->setRange(0,graph_max);
    if (data.yearly.max > 100) axisY->setLabelFormat("%.0f");

    chart->addAxis(axisY, Qt::AlignLeft);
    chart->addAxis(axis_X, Qt::AlignBottom);
    yearly_line->attachAxis(axis_X);

    yearly_line->attachAxis(axisY);


    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *y_linechart = new QChartView(chart);
    y_linechart->setRenderHint(QPainter::Antialiasing);
    y_linechart->setVisible(true);

    y_barchart->setMinimumHeight(300);
    y_linechart->setMinimumHeight(300);


    QScrollArea *scroll = new QScrollArea(this);
    QWidget *content = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(content);

    layout->addWidget(y_barchart);
    layout->addWidget(y_linechart);

    scroll->setWidget(content);
    scroll->setWidgetResizable(true);

    ui->y_linechart->addWidget(scroll);
}


YearlyPage::~YearlyPage()
{
    delete ui;
}
