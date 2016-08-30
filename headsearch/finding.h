#ifndef CFINDING_H
#define CFINDING_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class CFinding : public QDialog
{
    Q_OBJECT
public:
    explicit CFinding(QWidget *parent = 0);

signals:

public slots:

private:
    Ui::Dialog* ui;
};

#endif // CFINDING_H
