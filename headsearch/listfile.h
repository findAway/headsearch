#ifndef LISTFILE
#define LISTFILE

#include <QStringList>

/**
 * @brief 此类用于从给定的目录中列出所有包含文件，并将完整路径存入队列。
 */
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
    CListFile();
    ~CListFile();

    /**
     * @brief SeekFileType
     * @param fileType 需要查找的文件类型
     */
    void SeekFileType(emFileType fileType);

    /**
     * @brief AddSrcDir 添加查找目录
     * @param strDir 目录路径
     * @return 成功返回0
     */
    int AddSrcDir(const QString& strDir);

    /**
     * @brief CheckAndSeek 检查是否有未查找的目录，有就开始查找
     */
    void CheckAndSeek();

    /**
     * @brief SearchFile
     * @param strFileSearch 待查找文件名
     * @param pstrFileOut 输出查找到的文件名（完整路径）
     * @return 成功返回0
     */
    bool SearchFile(const QString& strFile, QString* pstrFilePathOut);

private:
    int LoopList(const QString& strDir, QStringList& strList);

private:
    emFileType  m_emSeekFileType;
    QStringList m_cSrcDirList;     //查找目录队列
    int         m_nListAtIndex;    //当前正在查找队列中的第几个

    QStringList m_cFileList;       //从目录中查找到的文件放入此队列
};

#endif // LISTFILE

