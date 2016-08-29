#ifndef CLOGFILE_H
#define CLOGFILE_H

#include <QFile>
#include <QString>
#include <QTextStream>

class CLogFile
{
public:
    CLogFile();
    ~CLogFile();

    int Open(const QString& file);
    void Write(const char* szFormat, ...);

private:
    QFile* m_pfile;
};

#endif // CLOGFILE_H
