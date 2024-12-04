#include "bluescreen.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BlueScreenDlg dlg;
    dlg.show();
    return a.exec();
}
