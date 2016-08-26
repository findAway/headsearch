#include <QFile>
#include <QByteArray>
#include "listinclude.h"

CListInclude::CListInclude()
{
    m_dwIncFiles = 0;
}

CListInclude::~CListInclude()
{
    m_dwIncFiles = 0;
}

int CListInclude::FindIncludes(const QString strFile, QStringList &incListOut)
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

        //查找 #include "headfile.h"
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
                for (int n = 0; n < incListOut.length(); n++)
                {
                    if (incListOut.at(n) == pStartPos)
                    {
                        bExist = true;
                        break;
                    }
                }

                if (!bExist)
                {
                    incListOut.append(QString::fromUtf8(pStartPos));
                    m_dwIncFiles++;
                }
            }
        }
    }

    file.close();

    return 0;
}
