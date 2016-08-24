#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QRegExp>
#include "listfile.h"

CListFile::CListFile(QString strDir)
{
    m_strStartDir = strDir;
    m_emSeekFileType = em_FileType_ALL;
}

CListFile::~CListFile()
{
    m_emSeekFileType = em_FileType_ALL;
}

void CListFile::SeekFileType(emFileType fileType)
{
    m_emSeekFileType = fileType;
}

int CListFile::StartList(QStringList& strListOut)
{
    return LoopList(m_strStartDir, strListOut);
}

int CListFile::LoopList(const QString strDir, QStringList& strList)
{
    QDir* pCurDir = new QDir(strDir);
    if (!pCurDir->exists())
    {
        delete pCurDir;
        pCurDir = 0;
        return 1;
    }

    pCurDir->setFilter(QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot);
    pCurDir->setSorting(QDir::Type);

    QFileInfoList fileInfoList = pCurDir->entryInfoList();
    for (int n = 0; n < fileInfoList.length(); n++)
    {
        QFileInfo fileInfo = fileInfoList.at(n);
        if (fileInfo.isDir())
        {
            LoopList(fileInfo.absoluteFilePath(), strList);
        }
        else
        {
            if (m_emSeekFileType == em_FileType_Head)
            {
                //QRegExp exp("*.h");
                if (fileInfo.absoluteFilePath().contains(".h"))
                {
                    strList.append(fileInfo.absoluteFilePath());
                }
            }
            else if (m_emSeekFileType == em_FileType_CPP)
            {
                //QRegExp expc("*.c");
                //QRegExp expcpp("*.cpp");

                if (fileInfo.absoluteFilePath().contains(".c"))
                {
                    strList.append(fileInfo.absoluteFilePath());
                }

                //if (fileInfo.absoluteFilePath().contains(expcpp))
                //{
                //    strList.append(fileInfo.absoluteFilePath());
                //}
            }
            else
            {
                strList.append(fileInfoList.at(n).absoluteFilePath());
            }
        }
    }

    delete pCurDir;
    pCurDir = 0;

    return 0;
}
