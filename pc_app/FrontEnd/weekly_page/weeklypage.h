#ifndef WEEKLYPAGE_H
#define WEEKLYPAGE_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>

namespace Ui  {
class WeeklyPage;
}

class WeeklyPage : public QWidget
{
    Q_OBJECT

public:
    explicit WeeklyPage(QWidget *parent = nullptr);
    ~WeeklyPage();

private:
    Ui::WeeklyPage *ui;
};

#endif // WEEKLYPAGE_H
