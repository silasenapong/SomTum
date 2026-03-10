#ifndef WEEKLYPAGE_H
#define WEEKLYPAGE_H

#include <QtGui>
#include <QtCore>
#include <QtWidgets>
#include <QtCharts>
<<<<<<< HEAD
=======
#include <QString>
#include <QValueAxis>
#include <dataManager/datamanager.h>
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64


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
<<<<<<< HEAD
=======
    DataManager data;
<<<<<<< HEAD
    void showData();
=======
>>>>>>> ab1d932cbd8446e71a64bf40432cbe93af9eae64
>>>>>>> 3a89fb51330640c0e8ba8622c6504dd0bff91c00
};

#endif // WEEKLYPAGE_H
