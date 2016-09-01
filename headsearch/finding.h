#ifndef CFINDING_H
#define CFINDING_H

#include <QDialog>
#include <QTimer>
#include <QMovie>

namespace Ui {
class Dialog;
}

class CFinding : public QDialog
{
    Q_OBJECT
public:
    explicit CFinding(QWidget *parent = 0);

public slots:
    void TimerOut();

signals:

private:
    Ui::Dialog* ui;
    QTimer* timer;
    int     nTimes;
};

#endif // CFINDING_H
