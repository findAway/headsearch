#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include <QRegExp>
#include "listfile.h"

CListFile::CListFile()
{
    m_emSeekFileType = em_FileType_ALL;
    m_nListAtIndex = 0;
}

CListFile::~CListFile()
{
    m_emSeekFileType = em_FileType_ALL;
    m_nListAtIndex = 0;
}

void CListFile::SeekFileType(emFileType fileType)
{
    m_emSeekFileType = fileType;
}

int CListFile::AddSrcDir(const QString& strDir)
{
    QDir dir(strDir);
    if (!dir.exists())
    {
        return 1;
    }

    m_cSrcDirList.append(strDir);
    return 0;
}

void CListFile::CheckAndSeek()
{
    if (m_cSrcDirList.length() == 0)
    {
        return;
    }

    while (m_nListAtIndex <= m_cSrcDirList.length()-1)
    {
        LoopList(m_cSrcDirList.at(m_nListAtIndex), m_cFileList);
        m_nListAtIndex++;
    }
}

bool CListFile::SearchFile(const QString& strFile, QString& strFilePathOut)
{
    for (int n = 0; n < m_cFileList.length(); n++)
    {
        QString strFilePath = m_cFileList.at(n);
        if (strFilePath.contains(strFile))
        {
            strFilePathOut = strFilePath;
            return true;
        }
    }

    return false;
}

int CListFile::LoopList(const QString& strDir, QStringList& strList)
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
