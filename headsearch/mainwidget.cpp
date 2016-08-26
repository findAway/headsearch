#include <QStringListModel>
#include <QLineEdit>
#include <QDir>
#include <QListView>
#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "listfile.h"
#include "listinclude.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    QObject::connect(ui->startButton, SIGNAL(clicked()), this, SLOT(DirCur()));
    m_pListViewModel = 0;
}

MainWidget::~MainWidget()
{
    delete ui;
    if(m_pListViewModel != 0)
    {
        delete m_pListViewModel;
        m_pListViewModel = 0;
    }
}

void MainWidget::DirCur()
{
    QListView* pFileListView = ui->listFileView;
    QLineEdit* pFileNumEdit = ui->lineEditFileNum;
    QLineEdit* pCurPathEdit = ui->lineEditCurPath;

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

    //显示当前路径
    pCurPathEdit->setText(QDir::currentPath());

    //显示文件数
    pFileNumEdit->setText(QString::number(strList.length(), 10));

    //列出各文件名（绝对路径）
    if (m_pListViewModel == 0)
    {
        m_pListViewModel = new QStringListModel(strList);
    }
    else
    {
        m_pListViewModel->setStringList(strList);
    }

    pFileListView->setModel(m_pListViewModel);
}
