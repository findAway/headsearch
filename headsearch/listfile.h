#ifndef LISTFILE
#define LISTFILE

#include <QStringList>

class CListFile
{
public:
    enum emFileType
    {
        em_FileType_Head,
        em_FileType_CPP,
        em_FileType_ALL
    };

public:
    CListFile(QString strDir);
    ~CListFile();

    /**
     * @brief SeekFileType
     * @param fileType 需要查找的文件类型
     */
    void SeekFileType(emFileType fileType);

    /**
     * @brief StartList
     * @param strListOut 输出参数
     * @return 成功返回0
     */
    int StartList(QStringList& strListOut);

    /**
     * @brief SearchFile
     * @param strFileList 存放文件名（完整路径）队列
     * @param strFileSearch 待查找文件名
     * @param pstrFileOut 输出查找到的文件名（完整路径）
     * @return 成功返回0
     */
    int SearchFile(const QStringList strFileList, QString strFileSearch, QString* pstrFileOut);

private:
    int LoopList(const QString strDir, QStringList& strList);

private:
    QString m_strStartDir;
    emFileType m_emSeekFileType;
};

#endif // LISTFILE

