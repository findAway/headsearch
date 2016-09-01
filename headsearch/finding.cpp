#include "finding.h"
#include "ui_finding.h"

CFinding::CFinding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    nTimes = 0;
    ui->labelfinding->setText(QObject::tr("处理中，请耐心等待"));
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerOut()));
    timer->start();
}

void CFinding::TimerOut()
{
    nTimes++;
    nTimes %= 4;

    if (nTimes == 1)
    {
        ui->labelfinding->setText(QObject::tr("处理中，请耐心等待."));
    }
    else if (nTimes == 2)
    {
        ui->labelfinding->setText(QObject::tr("处理中，请耐心等待.."));
    }
    else if (nTimes == 3)
    {
        ui->labelfinding->setText(QObject::tr("处理中，请耐心等待..."));
    }
    else
    {
        ui->labelfinding->setText(QObject::tr("处理中，请耐心等待"));
    }

    timer->start();
}
