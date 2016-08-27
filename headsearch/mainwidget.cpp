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
    m_pBtAddFirstFiles = ui->btAddFirstFiles;
    m_pBtStart = ui->btStart;

    m_pListViewSrcPath = ui->listViewSrcPath;
    m_pListViewFirstFiles = ui->listViewFirstFiles;
    m_pListViewPathOut = ui->listViewPathOut;
    m_pListViewSrcPath->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pListViewFirstFiles->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pListViewPathOut->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_pSrcPathViewModel = 0;
    m_pFirstFileViewModel = 0;
    m_pPathOutViewModel = 0;

    QObject::connect(m_pBtAddSrcPath, SIGNAL(clicked()), this, SLOT(AddSrcPath()));
    QObject::connect(m_pBtAddFirstFiles, SIGNAL(clicked()), this, SLOT(AddFirstFile()));
    QObject::connect(m_pBtStart, SIGNAL(clicked()), this, SLOT(DirCur()));
}

MainWidget::~MainWidget()
{
    delete ui;

    if (m_pSrcPathViewModel != 0)
    {
        delete m_pSrcPathViewModel;
        m_pSrcPathViewModel = 0;
    }

    if (m_pFirstFileViewModel != 0)
    {
        delete m_pFirstFileViewModel;
        m_pFirstFileViewModel = 0;
    }

    if (m_pPathOutViewModel != 0)
    {
        delete m_pPathOutViewModel;
        m_pPathOutViewModel = 0;
    }
}

void MainWidget::AddSrcPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Directory"), "./",
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

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

void MainWidget::AddFirstFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select File"), "./", tr("Head&Src Files (*.h *.c *.cpp)"));

    m_cFirstFileList.append(fileName);

    if (m_pFirstFileViewModel == 0)
    {
        m_pFirstFileViewModel = new QStringListModel(m_cFirstFileList);
    }
    else
    {
        m_pFirstFileViewModel->setStringList(m_cFirstFileList);
    }

    m_pListViewFirstFiles->setModel(m_pFirstFileViewModel);
}

void MainWidget::DirCur()
{
    if ((m_cSrcPathList.length() == 0) || (m_cFirstFileList.length() == 0))
    {
        QMessageBox::information(this, tr("警告"),
                                 tr("请添加文件搜寻路径及初始文件"), QMessageBox::Ok);

        return;
    }

    m_pBtAddSrcPath->setDisabled(true);
    m_pBtAddFirstFiles->setDisabled(true);
    m_pBtStart->setDisabled(true);

    //取出m_cSrcPathList队列中的目录，找出其中的所有文件
    CListFile listFile;
    listFile.SeekFileType(CListFile::em_FileType_Head);
    for (int n = 0; n < m_cSrcPathList.length(); n++)
    {
        listFile.AddSrcDir(m_cSrcPathList.at(n));
        listFile.CheckAndSeek();
    }

    //取出m_cFirstFileList队列中的文件，找出其包含的头文件
    CListInclude listIncs;
    for (int n = 0; n < m_cFirstFileList.length(); n++)
    {
        listIncs.AddNeededFile(m_cFirstFileList.at(n));
    }

    //循环查找各头文件的路径
    while (listIncs.HasHeadFile())
    {
        QString strHeadFile = listIncs.GetCurHeadFile();
        QString strFileOut;
        if (listFile.SearchFile(strHeadFile, &strFileOut))
        {
            listIncs.SetCurHeadFilePath(strFileOut);
        }

        listIncs.Next();
    }

    //取出所有已经查找到的头文件的完整路径
    QStringList strList;
    listIncs.GetOutAllHeadFilePath(strList);

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

    m_pBtAddSrcPath->setDisabled(false);
    m_pBtAddFirstFiles->setDisabled(false);
    m_pBtStart->setDisabled(false);
}
