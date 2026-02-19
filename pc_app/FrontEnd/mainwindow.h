#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>

#include "weekly_page/weeklypage.h"
#include "daily_page/dailypage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_weekly_button_clicked();
    void on_daily_button_clicked();


private:
    Ui::MainWindow *ui;
    WeeklyPage *weekly_page; // ptr
    DailyPage *daily_page;
};
#endif // MAINWINDOW_H
