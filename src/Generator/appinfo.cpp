#include "appinfo.h"
#include <QColor>
#include <QCoreApplication>
#include <QSettings>
#include <QTextCodec>

AppInfo *AppInfo::instance()
{
    static AppInfo info;
    return &info;
}

void AppInfo::save()
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    if (model.mEmojiType == EEmojiType::Char) {
        settings.setValue(KEY_EMOJI_CHAR, model.mEmojiCharacter);
        settings.setValue(KEY_EMOJI_PATH, "");
    } else {
        settings.setValue(KEY_EMOJI_CHAR, "");
        settings.setValue(KEY_EMOJI_PATH, model.mEmojiImgPath);
    }

    settings.setValue(KEY_MAIN_CONTENT, model.mMainContent);
    settings.setValue(KEY_CONTACT_HINT, model.mCttHint);
    settings.setValue(KEY_CONTACT_INFO, model.mCttInfo);
    settings.setValue(KEY_QRCODE, model.mQrcodePath);

    settings.setValue(KEY_BACKGROUND_COLOR, QString("%1,%2,%3").arg(model.mBgColor.red()).arg(model.mBgColor.green()).arg(model.mBgColor.blue()));
    settings.setValue(KEY_PROGRESS_TIME, model.progressTime);
    settings.setValue(KEY_HOT_KEY, model.mHotKey);
    settings.setValue(KEY_EXEC_CMD, model.cmd);
}

void AppInfo::reset()
{
    model.reset();
}

AppInfo::AppInfo()
    : iniPath(QCoreApplication::applicationDirPath() + "/" + "config.ini")
{
    init();
}

void AppInfo::init()
{
    loadConfig();
}

void AppInfo::loadConfig()
{
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("UTF-8"));

    model.mEmojiCharacter = settings.value(KEY_EMOJI_CHAR).toString();
    model.mEmojiImgPath = settings.value(KEY_EMOJI_PATH).toString();
    if (!model.mEmojiImgPath.isEmpty()) {
        model.mEmojiType = EEmojiType::Img;
    } else {
        model.mEmojiType = EEmojiType::Char;
    }

    model.mMainContent = settings.value(KEY_MAIN_CONTENT).toString();
    model.mCttHint = settings.value(KEY_CONTACT_HINT).toString();
    model.mCttInfo = settings.value(KEY_CONTACT_INFO).toString();
    model.mQrcodePath = settings.value(KEY_QRCODE).toString();

    QStringList colorList = settings.value(KEY_BACKGROUND_COLOR).toString().split(",");
    if (colorList.size() == 3) {
        model.mBgColor = QColor(colorList[0].toUInt(), colorList[1].toUInt(), colorList[2].toUInt());
    }
    model.progressTime = settings.value(KEY_PROGRESS_TIME).toUInt();
    model.mHotKey = settings.value(KEY_HOT_KEY).toString();
    model.cmd = settings.value(KEY_EXEC_CMD).toString();
}
