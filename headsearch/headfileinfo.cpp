#include "headfileinfo.h"
#include "ui_headfileinfo.h"

CHeadFileInfoWin::CHeadFileInfoWin() :
    QDialog(this),
    ui(new Ui::UiHeadFileInfo)
{
    ui->setupUi(this);

    m_pLineEditHeadFile = ui->lineEditHeadFile;
    m_pListViewFileInfo = ui->listWidgetFileInfo;

    m_pLineEditHeadFile->hide();
    m_pListViewFileInfo->hide();
}

void CHeadFileInfoWin::ShowHeadFile(const QString& file)
{
    m_pLineEditHeadFile->show();
    m_pLineEditHeadFile->setText(file);
}

void CHeadFileInfoWin::ShowContainFiles(const QStringList& list)
{
    m_pListViewFileInfo->show();
}
