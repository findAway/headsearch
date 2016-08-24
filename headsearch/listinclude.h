#ifndef CLISTINCLUDE_H
#define CLISTINCLUDE_H

#include <QStringList>

class CListInclude
{
public:
    CListInclude();
    ~CListInclude();

    /**
     * @brief FindIncludes
     * @param strFile 被查找的源文件名
     * @param incListOut 从源文件中找到的include文件将被放入此队列
     * @return 成功返回0
     */
    int FindIncludes(const QString strFile, QStringList& incListOut);

private:
    unsigned int m_dwIncFiles;
};

#endif // CLISTINCLUDE_H
