#include <QFile>
#include <QByteArray>
#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include "listinclude.h"

CListInclude::CListInclude()
{
    m_nListAtIndex = 0;
}

CListInclude::~CListInclude()
{
    m_nListAtIndex = 0;
    while (m_cHeadFileInfoList.length() > 0)
    {
        THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(0);
        m_cHeadFileInfoList.removeFirst();
        if (pHeadFileInfo != 0)
        {
            while (pHeadFileInfo->refFileList.length() > 0)
            {
                THeadRefFile* pRefHeadFile = pHeadFileInfo->refFileList.at(0);
                pHeadFileInfo->refFileList.removeFirst();
                if (pRefHeadFile != 0)
                {
                    delete pRefHeadFile;
                }
            }

            delete pHeadFileInfo;
        }
    }
}

int CListInclude::AddNeededFile(const QString& strFile)
{
    m_cNeededFileList.append(strFile);
    int nRet = FindIncludes(strFile, m_cNeededFileList.length()-1, true);

    return nRet;
}

int CListInclude::AddNeededPath(const QString& strPath)
{
    QDir* pCurDir = new QDir(strPath);
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
            AddNeededPath(fileInfo.absoluteFilePath());
        }
        else
        {
            if (fileInfo.absoluteFilePath().contains(".h") ||
                    fileInfo.absoluteFilePath().contains(".c") ||
                    fileInfo.absoluteFilePath().contains(".cc") ||
                    fileInfo.absoluteFilePath().contains(".cpp"))
            {
                m_cNeededFileList.append(fileInfo.absoluteFilePath());
            }
        }
    }

    delete pCurDir;
    pCurDir = 0;

    return 0;
}

int CListInclude::AddExterFile(const QString& strFile)
{
    m_cExterFileList.append(strFile);
    int nRet = FindIncludes(strFile, m_cExterFileList.length()-1, false);

    return nRet;
}

void CListInclude::FindAllIncludes()
{
    for (int n = 0; n < m_cNeededFileList.length(); n++)
    {
        //找出文件中包含的头文件
        FindIncludes(m_cNeededFileList.at(n), n, true);
    }
}

bool CListInclude::HasHeadFile()
{
    if (m_cHeadFileInfoList.length() <= 0)
    {
        return false;
    }

    if (m_nListAtIndex <= m_cHeadFileInfoList.length()-1)
    {
        return true;
    }

    return false;
}

const QString& CListInclude::GetCurHeadFile()
{
    THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(m_nListAtIndex);
    return pHeadFileInfo->strHeadFile;
}

void CListInclude::SetCurHeadFilePath(const QString& strHeadFilePath)
{
    THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(m_nListAtIndex);
    pHeadFileInfo->strHeadFilePath = strHeadFilePath;
    pHeadFileInfo->bSetPath = true;
}

void CListInclude::Next()
{
    if (m_nListAtIndex <= m_cHeadFileInfoList.length()-1)
    {
        m_nListAtIndex++;
    }
}

void CListInclude::SeekReset()
{
    m_nListAtIndex = 0;
}

const QStringList& CListInclude::GetOutAllNeedFiles()
{
//    for (int n = 0; n < m_cHeadFileInfoList.length(); n++)
//    {
//        THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(n);
//        strFilesListOut.append(pHeadFileInfo->strHeadFile);
//    }

    return m_cNeededFileList;
}

int CListInclude::DeleteNeedFile(int nFileIndex)
{
    if (nFileIndex >= m_cNeededFileList.length())
    {
        return 1;
    }

    m_cNeededFileList.removeAt(nFileIndex);
    return 0;
}

void CListInclude::GetOutAllHeadFilePath(QStringList& strPathListOut)
{
    for (int n = 0; n < m_cHeadFileInfoList.length(); n++)
    {
        THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(n);
        if (pHeadFileInfo->bSetPath)
        {
            strPathListOut.append(pHeadFileInfo->strHeadFilePath);
        }
        else
        {
            strPathListOut.append(pHeadFileInfo->strHeadFile + QObject::tr(" 未查找到"));
        }
    }
}

int CListInclude::FindIncludes(const QString& strFile, int nFileIndex, bool bInNeededList)
{
    QFile file(strFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 1;
    }

    while (!file.atEnd())
    {
        char buf[512];
        qint64 lineLength = file.readLine(buf, sizeof(buf));
        if (lineLength <= 0)
        {
            break;
        }

        //去掉最后一个换行符
        buf[lineLength - 1] = '\0';

        //查找 #include ""形式包含的头文件
        //有效的#include ""语句第一个字符是'#'
        bool bIncGramma = false;
        for (unsigned int n = 0; n < strlen(buf); n++)
        {
            if (buf[n] != ' ')
            {
                if (buf[n] == '#')
                {
                    bIncGramma = true;
                }
                else
                {
                    bIncGramma = false;
                }

                break;
            }
        }

        if (!bIncGramma)
        {
            continue;
        }

        QString str(buf);
        if (str.contains("#include") && str.contains("\""))
        {
            bool bStartMark = false;
            char* pStartPos = 0;

            //从#include "headfile.h"中截取出headfile.h
            for (unsigned int n = 0; n < strlen(buf); n++)
            {
                if (buf[n] == '\"' && bStartMark == false)
                {
                    bStartMark = true;
                    continue;
                }

                if (bStartMark && (pStartPos == 0) && (buf[n] != ' '))
                {
                    pStartPos = &buf[n];
                    continue;
                }

                if (bStartMark && (pStartPos != 0) && (buf[n] == '\"'))
                {
                    buf[n] = '\0';
                    break;
                }
            }

            if (pStartPos != 0)
            {
                QStringList splitList = QObject::tr(pStartPos).split("/");
                QString newHeadFile = splitList.last();

                //检查队列中是否存在相同文件名
                int nFileInfoIndex = -1;

                for (int n = 0; n < m_cHeadFileInfoList.length(); n++)
                {
                    THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(n);
                    if (pHeadFileInfo->strHeadFile.compare(newHeadFile) == 0)
                    {
                        nFileInfoIndex = n;
                        break;
                    }
                }

                if (nFileInfoIndex == -1)
                {
                    THeadFileInfo* pHeadFileInfo = new THeadFileInfo;

                    THeadRefFile* pHeadRefFile = new THeadRefFile;
                    pHeadRefFile->bInNeedList = bInNeededList;
                    pHeadRefFile->nRefFileIndex = nFileIndex;
                    pHeadFileInfo->refFileList.append(pHeadRefFile);

                    pHeadFileInfo->strHeadFile = newHeadFile;
                    pHeadFileInfo->bSetPath = false;

                    m_cHeadFileInfoList.append(pHeadFileInfo);
                }
                else
                {
                    THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(nFileInfoIndex);

                    THeadRefFile* pHeadRefFile = new THeadRefFile;
                    pHeadRefFile->bInNeedList = bInNeededList;
                    pHeadRefFile->nRefFileIndex = nFileIndex;
                    pHeadFileInfo->refFileList.append(pHeadRefFile);
                }
            }
        }
    }

    file.close();

    return 0;
}
