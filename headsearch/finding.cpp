#include "finding.h"
#include "ui_finding.h"

CFinding::CFinding(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}
