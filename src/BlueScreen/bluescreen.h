#ifndef BLUESCREEN_H
#define BLUESCREEN_H

#include <QDialog>
#include "../Common/def.h"

class QLabel;
class QVBoxLayout;
class BlueScreenDlg : public QDialog
{
    Q_OBJECT

public:
    BlueScreenDlg(bool isMainScreen,
                  QWidget *parent = nullptr);
    ~BlueScreenDlg();

private slots:
    void slotTimeout();
private:
    void initUi();
    void initTimer();

    void initEmojiUi();
    void initMainContentUi();
    void initProgressUi();
    void initContactUi();

    void updateProgress();
private:
    const AppInfoModel          &model;
    bool                        isMainScreen = false;
    QVBoxLayout*                mLayout = nullptr;
    QLabel*                     mProgressLbl = nullptr;
    int                         mProgress = 0;
    QTimer*                     mTimer;
};
#endif // BLUESCREEN_H
