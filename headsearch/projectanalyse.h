#ifndef CPROJECTANALYSE_H
#define CPROJECTANALYSE_H

#include <QString>
#include <QStringList>

class CHeadFileInfo
{
public:
    QString fileName;
    const QString* filePath;
    QList<const QString*> listOwnFile;
};

class CProjectAnalyse
{
public:
    CProjectAnalyse();
    ~CProjectAnalyse();

    int AddProjectPath(const QString& path);
    int AddSearchPath(const QString& path);
    int RemoveSearchFile(int fileIndex);
    int Process();

    QStringList& GetProjectPathList();
    QStringList& GetSearchFileList();
    void GetOutIncludeFiles(QStringList& list);

    const CHeadFileInfo* GetHeadFileInfo(int index);

private:
    void GetFilesFromPath(const QString& path, QStringList& list);
    void GetIncludeFromFile(const QString& file);
    const QString* FindFileFromList(const QStringList& list, const QString& file);
    bool IsExist(const QStringList& list, const QString& str);

private:
    QStringList m_listPojectPath;
    QStringList m_listProjectFiles;

    QStringList m_listSearchFile;
    QList<const QString*> m_listSearchFileMore;

    int m_nIncludeFileIndex;
    QList<CHeadFileInfo*> m_listIncludeFile;
};

#endif // CPROJECTANALYSE_H
