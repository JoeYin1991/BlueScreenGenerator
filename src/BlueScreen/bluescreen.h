#ifndef BLUESCREEN_H
#define BLUESCREEN_H

#include "../def.h"
#include <QDialog>

class QLabel;
class QVBoxLayout;
class BlueScreenDlg : public QDialog
{
    Q_OBJECT

public:
    BlueScreenDlg(QWidget *parent = nullptr);
    ~BlueScreenDlg();

private:
    void initUi();
    void initData();

    void initEmojiUi();
    void initMainContentUi();
    void initProgressUi();
    void initContactUi();

    void updateProgress();
private:
    const QString               iniPath;
    AppInfoModel                model;

    QVBoxLayout                 *mLayout = nullptr;
    QLabel                      *progressLbl = nullptr;
    int                         progress = 0;
};
#endif // BLUESCREEN_H
