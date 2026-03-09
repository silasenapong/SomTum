#ifndef YEARLYPAGE_H
#define YEARLYPAGE_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>
#include <QString>
#include <QValueAxis>
#include <dataManager/datamanager.h>

namespace Ui {
class YearlyPage;
}

class YearlyPage : public QWidget
{
    Q_OBJECT

public:
    explicit YearlyPage(QWidget *parent = nullptr);
    ~YearlyPage();

private:
    Ui::YearlyPage *ui;
    DataManager data;
};

#endif // YEARLYPAGE_H
