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
    void showData();
    DataManager data;

=======
<<<<<<< HEAD
=======
    DataManager data;
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
>>>>>>> 3a89fb51330640c0e8ba8622c6504dd0bff91c00
};



#endif // DAILYPAGE_H
