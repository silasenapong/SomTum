#ifndef MONTHLYPAGE_H
#define MONTHLYPAGE_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>
#include <QString>
#include <dataManager/datamanager.h>

namespace Ui {
class MonthlyPage;
}

class MonthlyPage : public QWidget
{
    Q_OBJECT

public:
    explicit MonthlyPage(QWidget *parent = nullptr);
    ~MonthlyPage();

private:
    Ui::MonthlyPage *ui;
    DataManager data;
    void showData();
};

#endif // MONTHLYPAGE_H
