#ifndef LISTFILE
#define LISTFILE

class CListFile
{
public:
    CListFile(QString strDir);
    ~CListFile();

    /**
     * @brief StartList
     * @param strListOut 输出参数
     * @return
     */
    int StartList(QStringList& strListOut);

private:
    int LoopList(const QString strDir, QStringList& strList);

private:
    QString m_strStartDir;
};

#endif // LISTFILE

