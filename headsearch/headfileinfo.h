#ifndef CHEADFILEINFO_H
#define CHEADFILEINFO_H

#include <QDialog>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringList>
#include <QStringListModel>

namespace Ui {
class UiHeadFileInfo;
}

class CHeadFileInfoWin : public QDialog
{
    Q_OBJECT
public:
    CHeadFileInfoWin(QWidget *parent = 0);
    void ShowHeadFile(const QString& file);
    void ShowHeadFilePath(const QString& path);
    void ShowContainFiles(const QStringList& list);

public slots:
    void OpenFile(const QModelIndex& index);

private:
    Ui::UiHeadFileInfo* ui;

    QLineEdit* m_pLineEditHeadFile;
    QLineEdit* m_pLineEditHeadFilePath;
    QListView* m_pListViewFileInfo;
    QStringListModel* m_pFileInfoViewModel;

    QStringList m_listFiles;
};

#endif // CHEADFILEINFO_H
