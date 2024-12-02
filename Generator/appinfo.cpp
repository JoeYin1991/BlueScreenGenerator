#include "appinfo.h"
#include <QColor>
#include <QCoreApplication>
#include <QSettings>
#include <QTextCodec>

const QString ini_name = "config.ini";

AppInfo *AppInfo::instance()
{
    static AppInfo info;
    return &info;
}

void AppInfo::save()
{
    QString path = QCoreApplication::applicationDirPath() + "/" + ini_name;
    QSettings settings(path, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    if (model.mEmojiType == EEmojiType::Char) {
        settings.setValue("emoji_char", model.mEmojiCharacter);
        settings.setValue("emoji_path", "");
    } else {
        settings.setValue("emoji_char", "");
        settings.setValue("emoji_path", model.mEmojiImgPath);
    }

    settings.setValue("main_content", model.mMainContent);
    settings.setValue("contact_hint", model.mCttHint);
    settings.setValue("contact_info", model.mCttInfo);
    settings.setValue("qrcode", model.mQrcodePath);

    settings.setValue("background_color", QString("%1,%2,%3").arg(model.mBgColor.red()).arg(model.mBgColor.green()).arg(model.mBgColor.blue()));
    settings.setValue("progress_time", model.progressTime);
    settings.setValue("hot_key", model.mHotKey);
    settings.setValue("execute_cmd", model.cmd);
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
    model.mBgColor = QColor(0, 120, 215);

    model.mHotKey = "ctrl+d";

}
