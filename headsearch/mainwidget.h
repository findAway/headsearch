#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QStringListModel>
#include <QListView>
#include "projectanalyse.h"

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
    void AddNeedPath();
    void DirCur();  //列出当前目录文件，包括子目录

    void DelNeedFile(const QModelIndex& index); //删除一项需要的文件

    void ShowPathOutInfo(const QModelIndex& index); //输出项的详情

private:
    Ui::MainWidget *ui;

    QPushButton*  m_pBtAddSrcPath;
    QPushButton*  m_pBtAddNeedPath;
    QPushButton*  m_pBtStart;

    QListView* m_pListViewSrcPath;
    QListView* m_pListViewNeedFiles;
    QListView* m_pListViewPathOut;

    QStringList m_cSrcPathList;
    QStringList m_cNeedPathList;
    QStringList m_cNeedFileList;

    QStringListModel* m_pSrcPathViewModel;
    QStringListModel* m_pNeedFileViewModel;
    QStringListModel* m_pPathOutViewModel;

    CProjectAnalyse m_cPrjAnalyser;
};

#endif // MAINWIDGET_H
