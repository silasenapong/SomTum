#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

<<<<<<< HEAD
=======
    // Add Page to Stacked Widgets

>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
    daily_page = new DailyPage(this);
    ui->stackedWidget->addWidget(daily_page);

    weekly_page = new WeeklyPage(this);
    ui->stackedWidget->addWidget(weekly_page);

<<<<<<< HEAD
    ui->stackedWidget->setCurrentWidget(daily_page);

=======
    monthly_page = new MonthlyPage(this);
    ui->stackedWidget->addWidget(monthly_page);

    yearly_page = new YearlyPage(this);
    ui->stackedWidget->addWidget(yearly_page);


    ui->stackedWidget->setCurrentWidget(daily_page);

    // button set up

    ui->daily_button->setCheckable(true);
    ui->weekly_button->setCheckable(true);
    ui->monthly_button->setCheckable(true);
    ui->yearly_button->setCheckable(true);

    QButtonGroup *group = new QButtonGroup(this);

    group->addButton(ui->daily_button);
    group->addButton(ui->weekly_button);
    group->addButton(ui->monthly_button);
    group->addButton(ui->yearly_button);

    group->setExclusive(true); // choose one button


}

    //button clicked

void MainWindow::on_daily_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(daily_page);
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
}

void MainWindow::on_weekly_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(weekly_page);
}

<<<<<<< HEAD
void MainWindow::on_daily_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(daily_page);
=======
void MainWindow::on_monthly_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(monthly_page);
}

void MainWindow::on_yearly_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(yearly_page);
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
}

MainWindow::~MainWindow()
{
    delete ui;
}
