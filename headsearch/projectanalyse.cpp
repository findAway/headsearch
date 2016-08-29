#include <QDir>
#include <QStringList>
#include <QFileInfo>
#include "projectanalyse.h"

CProjectAnalyse::CProjectAnalyse()
{
    m_nIncludeFileIndex = 0;
    m_cLogFile.Open(QObject::tr("./projectany.log"));
}

CProjectAnalyse::~CProjectAnalyse()
{
    for (int n = 0; n < m_listIncludeFile.length(); n++)
    {
        CHeadFileInfo* pHeadFileInfo = m_listIncludeFile.at(n);
        if (pHeadFileInfo != 0)
        {
            delete pHeadFileInfo;
        }
    }
}

int CProjectAnalyse::AddProjectPath(const QString& path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        return 1;
    }

    if (!IsExist(m_listPojectPath, path))
    {
        m_listPojectPath.append(path);
    }

    m_cLogFile.Write("添加工程文件: %s\n", path.toLatin1().constData());

    return 0;
}

int CProjectAnalyse::AddSearchPath(const QString& path)
{
    QDir dir(path);
    if (!dir.exists())
    {
        return 1;
    }

    GetFilesFromPath(path, m_listSearchFile);
    return 0;
}

int CProjectAnalyse::RemoveSearchFile(int fileIndex)
{
    if (fileIndex >= m_listSearchFile.length())
    {
        return 1;
    }

    m_listSearchFile.removeAt(fileIndex);
    return 0;
}

int CProjectAnalyse::Process()
{
    //找出工程目录下的所有文件加入队列
    for (int n = 0; n < m_listPojectPath.length(); n++)
    {
        GetFilesFromPath(m_listPojectPath.at(n), m_listProjectFiles);
    }

    //找出查找文件所包含的头文件加入队列m_listIncludeFile
    for (int n = 0; n < m_listSearchFile.length(); n++)
    {
        GetIncludeFromFile(m_listSearchFile.at(n));
    }

    //从队列m_listIncludeFile中取出头文件，找出头文件的路径
    while (m_nIncludeFileIndex <= m_listIncludeFile.length()-1)
    {
        CHeadFileInfo* ptHeadFileInfo = m_listIncludeFile.at(m_nIncludeFileIndex);
        m_nIncludeFileIndex++;
        if (ptHeadFileInfo == 0)
        {
            continue;
        }

        //先从m_listSearchFile查找，再从m_listProjectFiles中查找
        const QString* pFilePath = FindFileFromList(m_listSearchFile, ptHeadFileInfo->fileName);
        if (pFilePath != 0)
        {
            ptHeadFileInfo->filePath = pFilePath;
            continue;
        }

        pFilePath = FindFileFromList(m_listProjectFiles, ptHeadFileInfo->fileName);
        if (pFilePath != 0)
        {
            ptHeadFileInfo->filePath = pFilePath;

            //找到的头文件再次找出其包含的头文件
            m_listSearchFileMore.append(pFilePath);
            GetIncludeFromFile(*pFilePath);
        }
    }

    return 0;
}

QStringList& CProjectAnalyse::GetProjectPathList()
{
    return m_listPojectPath;
}

QStringList& CProjectAnalyse::GetSearchFileList()
{
    return m_listSearchFile;
}

void CProjectAnalyse::GetOutIncludeFiles(QStringList& list)
{
    for (int n = 0; n < m_listIncludeFile.length(); n++)
    {
        CHeadFileInfo* pHeadFileInfo = m_listIncludeFile.at(n);
        if (pHeadFileInfo->filePath != 0)
        {
            list.append(*(pHeadFileInfo->filePath));
            //list.append(pHeadFileInfo->fileName);
        }
        else
        {
            QString str = pHeadFileInfo->fileName + QObject::tr(" 未找到");
            list.append(str);
        }
    }
}

const CHeadFileInfo* CProjectAnalyse::GetHeadFileInfo(int index)
{
    if (index >= m_listIncludeFile.length())
    {
        return (CHeadFileInfo*)0;
    }

    return m_listIncludeFile.at(index);
}

void CProjectAnalyse::GetFilesFromPath(const QString& path, QStringList& list)
{
    QDir dir(path);
    if (!dir.exists())
    {
        return;
    }

    dir.setFilter(QDir::Files|QDir::AllDirs|QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Type);

    QFileInfoList fileInfoList = dir.entryInfoList();
    for (int n = 0; n < fileInfoList.length(); n++)
    {
        QFileInfo fileInfo = fileInfoList.at(n);
        if (fileInfo.isDir())
        {
            GetFilesFromPath(fileInfo.absoluteFilePath(), list);
        }
        else
        {
            if (fileInfo.absoluteFilePath().contains(".h") ||
                    fileInfo.absoluteFilePath().contains(".c") ||
                    fileInfo.absoluteFilePath().contains(".cc") ||
                    fileInfo.absoluteFilePath().contains(".cpp"))
            {
                if (!IsExist(list, fileInfo.absoluteFilePath()))
                {
                    list.append(fileInfo.absoluteFilePath());
                }
            }
        }
    }

    return;
}

void CProjectAnalyse::GetIncludeFromFile(const QString& file)
{
    QFile fFile(file);
    if (!fFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }

    while (!fFile.atEnd())
    {
        char buf[512];
        qint64 lineLength = fFile.readLine(buf, sizeof(buf));
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

                //找出队列中是否有相同文件
                int nSameIndex = -1;
                for (int n = 0; n < m_listIncludeFile.length(); n++)
                {
                    CHeadFileInfo* pHeadFileInfo = m_listIncludeFile.at(n);
                    if (pHeadFileInfo == 0)
                    {
                        continue;
                    }

                    if (pHeadFileInfo->fileName.compare(newHeadFile) == 0)
                    {
                        nSameIndex = n;
                        break;
                    }
                }

                if (nSameIndex != -1)
                {
                    CHeadFileInfo* pHeadFileInfo = m_listIncludeFile.at(nSameIndex);
                    pHeadFileInfo->listOwnFile.append(&file);
                }
                else
                {
                    CHeadFileInfo* pHeadFileInfo = new CHeadFileInfo;
                    pHeadFileInfo->fileName = newHeadFile;
                    pHeadFileInfo->listOwnFile.append(&file);

                    m_listIncludeFile.append(pHeadFileInfo);
                }
            }
        }
    }

    fFile.close();
}

const QString* CProjectAnalyse::FindFileFromList(const QStringList& list, const QString& file)
{
    for (int n = 0; n < list.length(); n++)
    {
        QString strFilePath = list.at(n);
        QStringList splitList = strFilePath.split("/");
        QString newHeadFile = splitList.last();
        if (newHeadFile.compare(file) == 0)
        {
            return &(list.at(n));
        }
    }

    return 0;
}

bool CProjectAnalyse::IsExist(const QStringList& list, const QString& str)
{
    for (int n = 0; n < list.length(); n++)
    {
        if (str.compare(list.at(n)) == 0)
        {
            return true;
        }
    }

    return false;
}
