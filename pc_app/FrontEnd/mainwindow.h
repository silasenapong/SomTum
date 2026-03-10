#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>

#include "weekly_page/weeklypage.h"
#include "daily_page/dailypage.h"
<<<<<<< HEAD
=======
#include "monthly_page/monthlypage.h"
#include "yearly_page/yearlypage.h"
#include "dataManager/datamanager.h"
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64

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
<<<<<<< HEAD
    void on_weekly_button_clicked();
    void on_daily_button_clicked();
=======
    void on_daily_button_clicked();
    void on_weekly_button_clicked();
    void on_monthly_button_clicked();
    void on_yearly_button_clicked();
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64


private:
    Ui::MainWindow *ui;
<<<<<<< HEAD
    WeeklyPage *weekly_page; // ptr
    DailyPage *daily_page;
=======
     // ptr
    DailyPage *daily_page;
    WeeklyPage *weekly_page;
    MonthlyPage *monthly_page;
    YearlyPage *yearly_page;
    DataManager data;



>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
};
#endif // MAINWINDOW_H
