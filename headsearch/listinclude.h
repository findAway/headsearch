#ifndef CLISTINCLUDE_H
#define CLISTINCLUDE_H

#include <QList>
#include <QStringList>

typedef struct tagHeadFileInfo
{
    QString strHeadFile;
    bool    bSetPath;
    QString strHeadFilePath;
} THeadFileInfo;

/**
 * @brief 此类用于从文件中找出所包含的头文件，并存入队列。
 * 以从webrtc源码中提取出x264编解码器的源码为例来说明此类的部分函数的用法。
 * 例如已知x264包含文件x264codec.h、x264codec.cpp，因为调用AddNeededFile
 * 将这2个文件的路径传入。传入后AddNeededFile会解析出其中所包含的头文件，假如
 * 有x264exer.h，找到x264exer.h的完整路径并传入AddExterFile。传入后AddExterFile
 * 同样会解析出其中所包含的头文件，依此进行下去，直到所有相关头文件都被找到。
 */
class CListInclude
{
public:
    CListInclude();
    ~CListInclude();

    /**
     * @brief AddNeededFile 添加真正需要的文件
     * @param strFile 文件名
     * @return 成功返回0
     */
    int AddNeededFile(const QString& strFile);

    /**
     * @brief AddExterFile 添加额外需要的文件
     * @param strFile 文件名
     * @return 成功返回0
     */
    int AddExterFile(const QString& strFile);

    /**
     * @brief 以下几个函数用于从m_cHeadFileList依次取出头文件。
     * HasHeadFile 判断是否还有未被取出的头文件。
     * GetCurHeadFile 取出m_nListAtIndex指向的头文件。
     * SetCurHeadFilePath 设置当前m_nListAtIndex指向的头文件的完整路径。
     * Next 让m_nListAtIndex指向下一个头文件的位置。
     * SeekReset 让m_nListAtIndex指向第一个头文件的位置。
     * @return
     */
    bool HasHeadFile();
    const QString& GetCurHeadFile();
    void SetCurHeadFilePath(const QString& strHeadFilePath);
    void Next();
    void SeekReset();

    /**
     * @brief 取出所有已经查找到的头文件的完整路径
     * @param strPathListOut 输出完整路径的队列
     */
    void GetOutAllHeadFilePath(QStringList& strPathListOut);

private:
    /**
     * @brief FindIncludes
     * @param strFile 被查找的源文件名
     * @return 成功返回0
     */
    int FindIncludes(const QString& strFile);

private:
    QStringList m_cNeededFileList;
    QStringList m_cExterFileList;

    //QStringList m_cHeadFileList;
    QList<THeadFileInfo*> m_cHeadFileInfoList;
    int         m_nListAtIndex;
};

#endif // CLISTINCLUDE_H
