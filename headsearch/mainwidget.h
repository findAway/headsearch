#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStringListModel>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

public slots:
    void DirCur();  //列出当前目录文件，包括子目录

private:
    Ui::MainWidget *ui;
    QStringListModel* m_pListViewModel;
};

#endif // MAINWIDGET_H
