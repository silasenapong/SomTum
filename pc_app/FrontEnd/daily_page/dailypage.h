#ifndef DAILYPAGE_H
#define DAILYPAGE_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>
#include <QString>

namespace Ui {
class DailyPage;
}

class DailyPage : public QWidget
{
    Q_OBJECT

public:
    explicit DailyPage(QWidget *parent = nullptr);
    ~DailyPage();

private:
    Ui::DailyPage *ui;
};

#endif // DAILYPAGE_H
