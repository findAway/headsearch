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
    CListFile listFile(QDir::currentPath());
    listFile.SeekFileType(CListFile::em_FileType_Head);
    QStringList allFilesList;
    listFile.StartList(allFilesList);

    //get headfiles from .cpp
    CListInclude listIncs;
    QStringList strHeadsList;
    listIncs.FindIncludes(QString("./mainwidget.cpp"), strHeadsList);

    //search headfiles from allFileList, add append to strList
    QStringList strList;
//    for (int n = 0; n < strHeadsList.length(); n++)
//    {
//        QString strFileOut;
//        int nRet = listFile.SearchFile(allFilesList, strHeadsList.at(n), &strFileOut);
//        if (nRet == 0)
//        {
//            strList.append(strFileOut);
//        }
//    }
    int n = 0;
    while (true)
    {
        if (n == strHeadsList.length())
        {
            break;
        }

        QString strFileOut;
        int nRet = listFile.SearchFile(allFilesList, strHeadsList.at(n), &strFileOut);
        if (nRet == 0)
        {
            strList.append(strFileOut);

            listIncs.FindIncludes(strFileOut, strHeadsList);
        }

        n++;
    }

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
