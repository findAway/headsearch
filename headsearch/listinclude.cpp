#include <QFile>
#include <QByteArray>
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
            delete pHeadFileInfo;
        }
    }
}

int CListInclude::AddNeededFile(const QString& strFile)
{
    int nRet = FindIncludes(strFile);
    if (nRet == 0)
    {
        m_cNeededFileList.append(strFile);
    }

    return nRet;
}

int CListInclude::AddExterFile(const QString& strFile)
{
    int nRet = FindIncludes(strFile);
    if (nRet == 0)
    {
        m_cExterFileList.append(strFile);
    }

    return nRet;
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

int CListInclude::FindIncludes(const QString& strFile)
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

                if (bStartMark && (pStartPos != 0) && (buf[n] == 'h'))
                {
                    buf[n+1] = '\0';
                    break;
                }
            }

            if (pStartPos != 0)
            {
                //检查队列中是否存在相同文件名
                bool bExist = false;
//                for (int n = 0; n < m_cHeadFileList.length(); n++)
//                {
//                    if (m_cHeadFileList.at(n) == pStartPos)
//                    {
//                        bExist = true;
//                        break;
//                    }
//                }

//                if (!bExist)
//                {
//                    m_cHeadFileList.append(QString::fromUtf8(pStartPos));
//                }

                for (int n = 0; n < m_cHeadFileInfoList.length(); n++)
                {
                    THeadFileInfo* pHeadFileInfo = m_cHeadFileInfoList.at(n);
                    if (pHeadFileInfo->strHeadFile == pStartPos)
                    {
                        bExist = true;
                        break;
                    }
                }

                if (!bExist)
                {
                    THeadFileInfo* pHeadFileInfo = new THeadFileInfo;
                    pHeadFileInfo->strHeadFile = QString::fromUtf8(pStartPos);
                    pHeadFileInfo->bSetPath = false;
                    m_cHeadFileInfoList.append(pHeadFileInfo);
                }
            }
        }
    }

    file.close();

    return 0;
}
