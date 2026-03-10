#ifndef DAILYPAGE_H
#define DAILYPAGE_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>
#include <QString>
<<<<<<< HEAD
=======
#include <QValueAxis>
#include "dataManager/datamanager.h"
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64

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
<<<<<<< HEAD
=======
    DataManager data;
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
};

#endif // DAILYPAGE_H
