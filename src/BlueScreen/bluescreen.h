#ifndef BLUESCREEN_H
#define BLUESCREEN_H

#include "../def.h"
#include <QDialog>

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
private:
    const QString               iniPath;
    AppInfoModel                model;

    QVBoxLayout                 *mLayout = nullptr;
};
#endif // BLUESCREEN_H
