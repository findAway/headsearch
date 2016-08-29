#include "logfile.h"

CLogFile::CLogFile()
{
    m_pfile = 0;
}

CLogFile::~CLogFile()
{
    if (m_pfile != 0)
    {
        m_pfile->close();
        delete m_pfile;
        m_pfile = 0;
    }
}

int CLogFile::Open(const QString& file)
{
    if (m_pfile != 0)
    {
        return 1;
    }

    m_pfile = new QFile(file);

    if (!m_pfile->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        delete m_pfile;
        m_pfile = 0;
        return 1;
    }

    return 0;
}

void CLogFile::Write(const char* szFormat, ...)
{
    if (m_pfile == 0)
    {
        return;
    }

    va_list pvList;
    va_start(pvList, szFormat);
    QString str = QString::vasprintf(szFormat, pvList);
    va_end(pvList);

    QTextStream stream(m_pfile);
    stream << str << endl;
}
