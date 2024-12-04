#ifndef BLUESCREEN_H
#define BLUESCREEN_H

#include "../def.h"
#include <QDialog>

class BlueScreenDlg : public QDialog
{
    Q_OBJECT

public:
    BlueScreenDlg(QWidget *parent = nullptr);
    ~BlueScreenDlg();

private:
    void initUi();
    void initData();
private:
    AppInfoModel    model;
};
#endif // BLUESCREEN_H
