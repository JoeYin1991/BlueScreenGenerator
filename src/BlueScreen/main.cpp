#include "bluescreen.h"
#include "../Common/appinfo.h"
#include <Windows.h>
#include <QApplication>
#include <QDebug>
#include <QSettings>

#define TASKMANAGER "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"

HHOOK keyHook=NULL;
HHOOK mouseHook=NULL;

void disableTaskManager(bool isDisable)
{
    QSettings settings(TASKMANAGER, QSettings::NativeFormat);
    if (isDisable) {
        settings.setValue("DisableTaskMgr", "1");
    }else {
        settings.remove("DisableTaskMgr");
    }
}


LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam )
{
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    if(nCode == HC_ACTION)
    {
        // if(pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL)& 0x8000 && GetAsyncKeyState(VK_SHIFT)&0x8000){
        //     qDebug() << "Ctrl+Shift+Esc";
        // }else if(pkbhs->vkCode == VK_ESCAPE && GetAsyncKeyState(VK_CONTROL) & 0x8000){
        //     qDebug() << "Ctrl+Esc";
        // }else if(pkbhs->vkCode == VK_TAB && pkbhs->flags & LLKHF_ALTDOWN){
        //     qDebug() << "Alt+Tab";
        // }else if(pkbhs->vkCode == VK_ESCAPE && pkbhs->flags &LLKHF_ALTDOWN){
        //     qDebug() << "Alt+Esc";
        // }else if(pkbhs->vkCode == VK_LWIN || pkbhs->vkCode == VK_RWIN){
        //     qDebug() << "LWIN/RWIN";
        // }else if(pkbhs->vkCode == VK_F4 && pkbhs->flags & LLKHF_ALTDOWN){
        //     qDebug() << "Alt+F4";
        // }
        if(AppInfo::instance()->isHotKeyPressed(pkbhs))
        {
            qApp->quit();
        }
    }
    return 1;
    // return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam )
{
    return 1;
}

void unHook()
{
    // disableTaskManager(false);
    UnhookWindowsHookEx(keyHook);
    UnhookWindowsHookEx(mouseHook);
}

void setHook()
{
    // disableTaskManager(true);
    keyHook =SetWindowsHookEx(WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);
    mouseHook =SetWindowsHookEx( WH_MOUSE_LL,mouseProc,GetModuleHandle(NULL),0);
}

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

    AppInfo::instance();

    setHook();
    ShowCursor(false);
    int numbers = GetSystemMetrics(SM_CMONITORS);
    QVector<BlueScreenDlg*> dlgVec;
    for (int i = 0; i < numbers; ++i) {
        auto rc = GetScreenRect(i);
        bool isMainScreen = rc.x() == 0 && rc.y() == 0;
        BlueScreenDlg *dlg = new BlueScreenDlg(isMainScreen);
        dlgVec.append(dlg);
        dlg->show();
        SetWindowPos(reinterpret_cast<HWND>(dlg->winId()), HWND_TOPMOST, rc.x(), rc.y(), rc.width(), rc.height(), NULL);
    }

    int retCode = a.exec();
    unHook();
    return retCode;
}
