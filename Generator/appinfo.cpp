#include "appinfo.h"
#include <QColor>

AppInfo *AppInfo::instance()
{
    static AppInfo info;
    return &info;
}

AppInfo::AppInfo()
{
    init();
}

void AppInfo::init()
{
    model.mEmojiType = EEmojiType::Char;
    model.mEmojiCharacter = ":(";

    model.mMainContent = "Your PC ran into a problem and needs to restart. We're just collecting some error info, and then we'll restart for you.";

    model.mCttHint = "For more information about this issue and possible fixes, visit https://www.windows.com/stopcode";
    model.mCttInfo = "If you call a support person, give them this info: Stop code: CRITICAL_PROCESS_DIED";

    QRgba64 rgba64;
    rgba64.setRed(0);
    rgba64.setGreen(120);
    rgba64.setBlue(215);
    model.mBgColor = rgba64.toArgb32();
}
