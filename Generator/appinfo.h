#ifndef APPINFO_H
#define APPINFO_H

#include <QObject>

enum class EEmojiType {
    Char,
    Img
};

struct AppInfoModel
{
    EEmojiType      mEmojiType = EEmojiType::Char;
    QString         mEmojiCharacter;
    QString         mEmojiImgPath;

    QString         mMainContent;

    QString         mQrcodePath;
    QString         mCttHint;
    QString         mCttInfo;

    uint            mBgColor;
    QString         mHotKey;
    int             progressTime;
    QString         cmd;
};

class AppInfo
{
public:
    static AppInfo *instance();
    const AppInfoModel& getModel(){return model;};
private:
    AppInfo();
    void init();
private:
    AppInfoModel    model;
};

#endif // APPINFO_H
