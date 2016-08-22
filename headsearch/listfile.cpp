#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include "listfile.h"

CListFile::CListFile(QString strDir)
{
    m_strStartDir = strDir;
}

CListFile::~CListFile()
{
}

int CListFile::StartList(QStringList& strListOut)
{
    LoopList(m_strStartDir, strListOut);
    return 0;
}

int CListFile::LoopList(const QString strDir, QStringList& strList)
{
    QDir* pCurDir = new QDir(strDir);
    pCurDir->setFilter(QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot);
    pCurDir->setSorting(QDir::Type);

    QFileInfoList fileInfoList = pCurDir->entryInfoList();
    for (int n = 0; n < fileInfoList.length(); n++)
    {
        if (fileInfoList.at(n).isDir())
        {
            LoopList(fileInfoList.at(n).absoluteFilePath(), strList);
        }
        else
        {
            strList.append(fileInfoList.at(n).absoluteFilePath());
        }
    }

    delete pCurDir;
    pCurDir = 0;

    return 0;
}
