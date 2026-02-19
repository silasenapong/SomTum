#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    daily_page = new DailyPage(this);
    ui->stackedWidget->addWidget(daily_page);

    weekly_page = new WeeklyPage(this);
    ui->stackedWidget->addWidget(weekly_page);

    ui->stackedWidget->setCurrentWidget(daily_page);

}

void MainWindow::on_weekly_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(weekly_page);
}

void MainWindow::on_daily_button_clicked()
{
    ui->stackedWidget->setCurrentWidget(daily_page);
}

MainWindow::~MainWindow()
{
    delete ui;
}
