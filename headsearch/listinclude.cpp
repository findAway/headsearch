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
//        QByteArray line = file.readLine();
//        if (line.length() == 0)
//        {
//            break;
//        }

//        QString str = QString::fromUtf8(line);
//        if (str.contains("#include \""))
//        {
//            incListOut.append(str);
//            m_dwIncFiles++;
//        }

        char buf[512];
        qint64 lineLength = file.readLine(buf, sizeof(buf));
        if (lineLength <= 0)
        {
            break;
        }

        //去掉最后一个换行符
        buf[lineLength - 1] = '\0';

        //查找#include""
        QString str(buf);
        if (str.contains("#include") && str.contains("\""))
        {
            incListOut.append(str);
            m_dwIncFiles++;
        }
    }

    file.close();

    return 0;
}
