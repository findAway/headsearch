#include <QMessageBox>
#include <QProcess>
#include "headfileinfo.h"
#include "ui_headfileinfo.h"

CHeadFileInfoWin::CHeadFileInfoWin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UiHeadFileInfo)
{
    ui->setupUi(this);

    m_pLineEditHeadFile = ui->lineEditHeadFile;
    m_pLineEditHeadFilePath = ui->lineEditHeadFilePath;
    m_pListViewFileInfo = ui->listViewContainFile;
    m_pFileInfoViewModel = 0;

    QObject::connect(m_pListViewFileInfo, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(OpenFile(QModelIndex)));
}

void CHeadFileInfoWin::ShowHeadFile(const QString& file)
{
    m_pLineEditHeadFile->setText(file);
}

void CHeadFileInfoWin::ShowHeadFilePath(const QString& path)
{
    m_pLineEditHeadFilePath->setText(path);
}

void CHeadFileInfoWin::ShowContainFiles(const QStringList& list)
{
    m_listFiles = list;
    if (m_pFileInfoViewModel == 0)
    {
        m_pFileInfoViewModel = new QStringListModel(m_listFiles);
    }
    else
    {
        m_pFileInfoViewModel->setStringList(m_listFiles);
    }

    m_pListViewFileInfo->setModel(m_pFileInfoViewModel);
}

void CHeadFileInfoWin::OpenFile(const QModelIndex& index)
{
    int nRow = index.row();
    if (nRow >= m_listFiles.length())
    {
        return;
    }

    do
    {
        int nChose = QMessageBox::information(this, tr("确定要打开以下文件吗？"),
                                              m_listFiles.at(nRow),
                                              QMessageBox::Ok|QMessageBox::No);
        if (nChose != QMessageBox::Ok)
        {
            break;
        }

        QString cmd = QObject::tr("C:/Program Files/Notepad++/notepad++.exe ");
        QStringList list;
        list.append(m_listFiles.at(nRow));
        if (QProcess::startDetached(cmd, list))
        {
            break;
        }

        cmd = QObject::tr("C:/Program Files (x86)/Notepad++/notepad++.exe ");
        if (QProcess::startDetached(cmd, list))
        {
            break;
        }

        cmd = QObject::tr("notepad.exe ");
        if (QProcess::startDetached(cmd, list))
        {
            break;
        }

        QMessageBox::information(this, tr("警告"),
                                 QObject::tr("文件打开失败"),
                                 QMessageBox::Ok);
    }while(0);
}
