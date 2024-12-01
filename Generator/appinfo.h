#ifndef APPINFO_H
#define APPINFO_H

#include <QObject>
#include <QColor>

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

    QColor          mBgColor;
    QString         mHotKey;
    int             progressTime;
    QString         cmd;
};

class AppInfo
{
public:
    static AppInfo *instance();
    AppInfoModel& getModel(){return model;};

    void save();
private:
    AppInfo();
    void init();
private:
    AppInfoModel    model;
};

#endif // APPINFO_H
