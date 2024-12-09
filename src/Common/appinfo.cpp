#include "appinfo.h"
#include "qkeyWinVcode.h"
#include <QColor>
#include <QCoreApplication>
#include <QSettings>
#include <QTextCodec>
#include <QFileInfo>
#include <QKeySequence>
#include <QDebug>

constexpr int keyMask = ~(Qt::META | Qt::CTRL | Qt::ALT | Qt::SHIFT);

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
    settings.setValue(KEY_FONT_COLOR, QString("%1,%2,%3").arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    settings.setValue(KEY_PROGRESS_TIME, model.progressTime);
    settings.setValue(KEY_HOT_KEY, model.mHotKey);
    settings.setValue(KEY_EXEC_CMD, model.cmd);
}

void AppInfo::reset()
{
    model.reset();
}

bool AppInfo::isHotKeyPressed(KBDLLHOOKSTRUCT *pkbhs)
{
    QStringList hotkeys = model.mHotKey.toLower().split("+");
    QKeySequence keySeq(model.mHotKey);
    for(int i = 0; i < keySeq.count(); i++)
    {
        int key = keySeq[i];
        Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(key & ~keyMask);
        Qt::Key singleKey = static_cast<Qt::Key>(key & keyMask);
        do
        {
            if (modifiers & Qt::ControlModifier && !(GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
                break;
            }
            if (modifiers & Qt::ShiftModifier && !(GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
                break;
            }
            if (modifiers & Qt::AltModifier && !(GetAsyncKeyState(VK_MENU) & 0x8000)) {
                break;
            }
            if (modifiers & Qt::MetaModifier  && !(GetAsyncKeyState(VK_LWIN) & 0x8000) && !(GetAsyncKeyState(VK_RWIN) & 0x8000)) {
                break;
            }
            if (qtKey2WinCodeMap.at(singleKey) == pkbhs->vkCode) {
                return true;
            }
        } while(0);
    }

    return false;
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
    if (!QFileInfo::exists(iniPath)) {
        return;
    }
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
    colorList = settings.value(KEY_FONT_COLOR).toString().split(",");
    if (colorList.size() == 3) {
        model.mFontColor = QColor(colorList[0].toUInt(), colorList[1].toUInt(), colorList[2].toUInt());
    }
    model.progressTime = settings.value(KEY_PROGRESS_TIME).toUInt();
    model.mHotKey = settings.value(KEY_HOT_KEY).toString();
    model.cmd = settings.value(KEY_EXEC_CMD).toString();
}
