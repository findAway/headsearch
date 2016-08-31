#ifndef CHEADFILEINFO_H
#define CHEADFILEINFO_H

#include <QDialog>
#include <QLineEdit>
#include <QListView>
#include <QString>
#include <QStringList>

namespace Ui {
class UiHeadFileInfo;
}

class CHeadFileInfoWin : public QDialog
{
public:
    CHeadFileInfoWin();
    void ShowHeadFile(const QString& file);
    void ShowContainFiles(const QStringList& list);

private:
    Ui::UiHeadFileInfo* ui;

    QLineEdit* m_pLineEditHeadFile;
    QListView* m_pListViewFileInfo;
};

#endif // CHEADFILEINFO_H
