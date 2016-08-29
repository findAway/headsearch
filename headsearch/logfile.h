#ifndef CLOGFILE_H
#define CLOGFILE_H

#include <QFile>
#include <QString>
#include <QTextStream>

#define STR2CHAR(str) (str.toLatin1().constData())

class CLogFile
{
public:
    CLogFile();
    ~CLogFile();

    int Open(const QString& file);
    void WriteLine(const char* szFormat, ...);

private:
    QFile* m_pfile;
};

#endif // CLOGFILE_H
