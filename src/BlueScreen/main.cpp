#include "bluescreen.h"
#include <Windows.h>
#include <QApplication>
#include <QDebug>

QRect GetScreenRect(int screen) {

    DISPLAY_DEVICEW device;
    device.cb = sizeof(device);
    EnumDisplayDevicesW(NULL, screen, &device, 0);

    DEVMODEW device_mode;
    device_mode.dmSize = sizeof(device_mode);
    device_mode.dmDriverExtra = 0;
    EnumDisplaySettingsExW(device.DeviceName, ENUM_CURRENT_SETTINGS, &device_mode, 0);

    int x = device_mode.dmPosition.x;
    int y = device_mode.dmPosition.y;
    int width = device_mode.dmPelsWidth;
    int height = device_mode.dmPelsHeight;

    return { x, y, width, height };
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication a(argc, argv);

    int numbers = GetSystemMetrics(SM_CMONITORS);
    QVector<BlueScreenDlg*> dlgVec;
    for (int i = 0; i < numbers; ++i) {
        BlueScreenDlg *dlg = new BlueScreenDlg;
        dlgVec.append(dlg);
        auto rc = GetScreenRect(i);
        if (rc.x() != 0 || rc.y() != 0) {
            continue;
        }
        dlg->show();
        SetWindowPos(reinterpret_cast<HWND>(dlg->winId()), NULL, rc.x(), rc.y(), rc.width(), rc.height(), NULL);
    }
    return a.exec();
}
