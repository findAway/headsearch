#include <QStringListModel>
#include <QLineEdit>
#include <QDir>
#include <QListView>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    m_pBtAddSrcPath = ui->btAddSrcPath;
    m_pBtAddNeedPath = ui->btAddNeedPath;
    m_pBtStart = ui->btStart;

    m_pListViewSrcPath = ui->listViewSrcPath;
    m_pListViewNeedFiles = ui->listViewFirstFiles;
    m_pListViewPathOut = ui->listViewPathOut;
    m_pListViewSrcPath->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pListViewNeedFiles->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pListViewPathOut->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pSrcPathViewModel = 0;
    m_pNeedFileViewModel = 0;
    m_pPathOutViewModel = 0;

    QObject::connect(m_pBtAddSrcPath, SIGNAL(clicked()), this, SLOT(AddSrcPath()));
    QObject::connect(m_pBtAddNeedPath, SIGNAL(clicked()), this, SLOT(AddNeedPath()));
    QObject::connect(m_pBtStart, SIGNAL(clicked()), this, SLOT(DirCur()));

    QObject::connect(m_pListViewNeedFiles, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(DelNeedFile(QModelIndex)));
    QObject::connect(m_pListViewPathOut, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(ShowPathOutInfo(QModelIndex)));
}

MainWidget::~MainWidget()
{
    delete ui;

    if (m_pSrcPathViewModel != 0)
    {
        delete m_pSrcPathViewModel;
        m_pSrcPathViewModel = 0;
    }

    if (m_pNeedFileViewModel != 0)
    {
        delete m_pNeedFileViewModel;
        m_pNeedFileViewModel = 0;
    }

    if (m_pPathOutViewModel != 0)
    {
        delete m_pPathOutViewModel;
        m_pPathOutViewModel = 0;
    }
}

void MainWidget::AddSrcPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("请选择目录"), "./",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
    {
        return;
    }

    m_cPrjAnalyser.AddProjectPath(dir);

    if (m_pSrcPathViewModel == 0)
    {
        m_pSrcPathViewModel = new QStringListModel(m_cPrjAnalyser.GetProjectPathList());
    }
    else
    {
        m_pSrcPathViewModel->setStringList(m_cPrjAnalyser.GetProjectPathList());
    }

    m_pListViewSrcPath->setModel(m_pSrcPathViewModel);
}

void MainWidget::AddNeedPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("请选择目录"), "./",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
    {
        return;
    }

    m_cPrjAnalyser.AddSearchPath(dir);

    //取出所有包含的文件显示出来
    if (m_pNeedFileViewModel == 0)
    {
        m_pNeedFileViewModel = new QStringListModel(m_cPrjAnalyser.GetSearchFileList());
    }
    else
    {
        m_pNeedFileViewModel->setStringList(m_cPrjAnalyser.GetSearchFileList());
    }

    m_pListViewNeedFiles->setModel(m_pNeedFileViewModel);
}

void MainWidget::DirCur()
{
    m_pBtAddSrcPath->setEnabled(false);
    m_pBtAddNeedPath->setEnabled(false);
    m_pBtStart->setEnabled(false);

    m_cPrjAnalyser.Process();

    //取出所有已经查找到的头文件的完整路径
    QStringList strList;
    m_cPrjAnalyser.GetOutIncludeFiles(strList);

    if (m_pPathOutViewModel == 0)
    {
        m_pPathOutViewModel = new QStringListModel(strList);
    }
    else
    {
        m_pPathOutViewModel->setStringList(strList);
    }

    m_pListViewPathOut->setModel(m_pPathOutViewModel);

    QMessageBox::information(this, tr("通知"),
                             tr("处理结束"), QMessageBox::Ok);
}

void MainWidget::DelNeedFile(const QModelIndex & index)
{
    int nRow = index.row();
    int nRet = QMessageBox::information(this, tr("警告"),
                                        tr("确定删除此文件吗?"), QMessageBox::Ok|QMessageBox::No, QMessageBox::No);

    if (nRet == QMessageBox::No)
    {
        return;
    }

    m_cPrjAnalyser.RemoveSearchFile(nRow);

    //取出所有包含的文件显示出来
    if (m_pNeedFileViewModel == 0)
    {
        m_pNeedFileViewModel = new QStringListModel(m_cPrjAnalyser.GetSearchFileList());
    }
    else
    {
        m_pNeedFileViewModel->setStringList(m_cPrjAnalyser.GetSearchFileList());
    }

    m_pListViewNeedFiles->setModel(m_pNeedFileViewModel);
}

void MainWidget::ShowPathOutInfo(const QModelIndex& index)
{
    int nRow = index.row();

    const CHeadFileInfo* pHeadFileInfo = m_cPrjAnalyser.GetHeadFileInfo(nRow);
    if (pHeadFileInfo == 0)
    {
        QMessageBox::information(this, tr("头文件信息"),
                                 tr("未找到引用文件"), QMessageBox::Ok);
    }
    else
    {
        QString info;
        info += tr("引用文件：");
        for (int n = 0; n < pHeadFileInfo->listOwnFile.length(); n++)
        {
            info += tr("\n");
            info += *(pHeadFileInfo->listOwnFile.at(n));
        }

        QMessageBox::information(this, tr("头文件信息"),
                                 info,
                                 QMessageBox::Ok);
    }
}
