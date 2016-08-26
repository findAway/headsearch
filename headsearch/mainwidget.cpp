#include <QStringListModel>
#include <QLineEdit>
#include <QDir>
#include <QListView>
#include <QFileDialog>
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

    m_pSrcPathViewModel = 0;
    m_pFirstFileViewModel = 0;

    QObject::connect(m_pBtAddSrcPath, SIGNAL(clicked()), this, SLOT(AddSrcPath()));
    QObject::connect(m_pBtAddFirstFiles, SIGNAL(clicked()), this, SLOT(AddFirstFile()));
    QObject::connect(m_pBtStart, SIGNAL(clicked()), this, SLOT(DirCur()));
}

MainWidget::~MainWidget()
{
    delete ui;
//    if(m_pListViewModel != 0)
//    {
//        delete m_pListViewModel;
//        m_pListViewModel = 0;
//    }
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
    m_pBtAddSrcPath->setDisabled(true);
    m_pBtAddFirstFiles->setDisabled(true);
    m_pBtStart->setDisabled(true);

    //list all head files into allFilesList
    CListFile listFile;
    listFile.SeekFileType(CListFile::em_FileType_Head);
    listFile.AddSrcDir(QDir::currentPath());
    listFile.CheckAndSeek();

    //get headfiles from .cpp
    CListInclude listIncs;
    listIncs.AddNeededFile(QString("./mainwidget.cpp"));

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

//    //显示当前路径
//    pCurPathEdit->setText(QDir::currentPath());

//    //显示文件数
//    pFileNumEdit->setText(QString::number(strList.length(), 10));

//    //列出各文件名（绝对路径）
//    if (m_pListViewModel == 0)
//    {
//        m_pListViewModel = new QStringListModel(strList);
//    }
//    else
//    {
//        m_pListViewModel->setStringList(strList);
//    }

//    pFileListView->setModel(m_pListViewModel);
}
