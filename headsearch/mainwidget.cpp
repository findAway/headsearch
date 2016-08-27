#include <QStringListModel>
#include <QLineEdit>
#include <QDir>
#include <QListView>
#include <QFileDialog>
#include <QMessageBox>
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "listfile.h"
#include "listinclude.h"

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

    //判断是否已经存在
    for (int n = 0; n < m_cSrcPathList.length(); n++)
    {
        if (dir.compare(m_cSrcPathList.at(n)) == 0)
        {
            return;
        }
    }

    m_cSrcPathList.append(dir);

    if (m_pSrcPathViewModel == 0)
    {
        m_pSrcPathViewModel = new QStringListModel(m_cSrcPathList);
    }
    else
    {
        m_pSrcPathViewModel->setStringList(m_cSrcPathList);
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

    //判断是否已经存在
    for (int n = 0; n < m_cNeedPathList.length(); n++)
    {
        if (dir.compare(m_cNeedPathList.at(n)) == 0)
        {
            return;
        }
    }

    m_cNeedPathList.append(dir);

    //将路径传入m_cListInclude找出所有包含文件
    m_cListInclude.AddNeededPath(dir);

    //取出所有包含的文件显示出来
    if (m_pNeedFileViewModel == 0)
    {
        m_pNeedFileViewModel = new QStringListModel(m_cListInclude.GetOutAllNeedFiles());
    }
    else
    {
        m_pNeedFileViewModel->setStringList(m_cListInclude.GetOutAllNeedFiles());
    }

    m_pListViewNeedFiles->setModel(m_pNeedFileViewModel);
}

void MainWidget::DirCur()
{
    if ((m_cSrcPathList.length() == 0) || (m_cNeedPathList.length() == 0))
    {
        QMessageBox::information(this, tr("警告"),
                                 tr("请添加文件搜寻路径及初始文件"), QMessageBox::Ok);

        return;
    }

    m_pBtAddSrcPath->setDisabled(true);
    m_pBtAddNeedPath->setDisabled(true);
    m_pBtStart->setDisabled(true);

    //取出m_cSrcPathList队列中的目录，找出其中的所有文件
    m_cListFiles.SeekFileType(CListFile::em_FileType_Head);
    for (int n = 0; n < m_cSrcPathList.length(); n++)
    {
        m_cListFiles.AddSrcDir(m_cSrcPathList.at(n));
        m_cListFiles.CheckAndSeek();
    }

    //循环查找各头文件的路径
    m_cListInclude.FindAllIncludes();
    while (m_cListInclude.HasHeadFile())
    {
        QString strHeadFile = m_cListInclude.GetCurHeadFile();
        QString strFileOut;
        if (m_cListFiles.SearchFile(strHeadFile, strFileOut))
        {
            m_cListInclude.SetCurHeadFilePath(strFileOut);
            m_cListInclude.AddExterFile(strFileOut);
        }

        m_cListInclude.Next();
    }

    //取出所有已经查找到的头文件的完整路径
    QStringList strList;
    m_cListInclude.GetOutAllHeadFilePath(strList);

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

    //    m_pBtAddSrcPath->setDisabled(false);
    //    m_pBtAddNeedPath->setDisabled(false);
    //    m_pBtStart->setDisabled(false);
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

    m_cListInclude.DeleteNeedFile(nRow);

    //取出所有包含的文件显示出来
    if (m_pNeedFileViewModel == 0)
    {
        m_pNeedFileViewModel = new QStringListModel(m_cListInclude.GetOutAllNeedFiles());
    }
    else
    {
        m_pNeedFileViewModel->setStringList(m_cListInclude.GetOutAllNeedFiles());
    }

    m_pListViewNeedFiles->setModel(m_pNeedFileViewModel);
}

void MainWidget::ShowPathOutInfo(const QModelIndex& index)
{
    int nRow = index.row();

    QStringList headFileInfoList;
    m_cListInclude.HeadFileMoreInfo(nRow, headFileInfoList);
    if (headFileInfoList.length() == 0)
    {
        QMessageBox::information(this, tr("警告"),
                                 tr("为找到引用文件"), QMessageBox::Ok);
    }
    else
    {
        QString info;
        info += tr("引用文件：");
        for (int n = 0; n < headFileInfoList.length(); n++)
        {
            info += tr("\n");
            info += headFileInfoList.at(n);
        }

        QMessageBox::information(this, tr("警告"),
                                 info,
                                 QMessageBox::Ok);
    }
}
