#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QStringListModel>
#include <QListView>

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
    void AddSrcPath();
    void AddFirstFile();
    void DirCur();  //列出当前目录文件，包括子目录

private:
    Ui::MainWidget *ui;

    QPushButton*  m_pBtAddSrcPath;
    QPushButton*  m_pBtAddFirstFiles;
    QPushButton*  m_pBtStart;

    QListView* m_pListViewSrcPath;
    QListView* m_pListViewFirstFiles;
    QListView* m_pListViewPathOut;

    QStringList m_cSrcPathList;
    QStringList m_cFirstFileList;

    QStringListModel* m_pSrcPathViewModel;
    QStringListModel* m_pFirstFileViewModel;
    QStringListModel* m_pPathOutViewModel;
};

#endif // MAINWIDGET_H
