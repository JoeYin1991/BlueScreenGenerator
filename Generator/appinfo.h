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
    EEmojiType      mEmojiType;
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

    AppInfoModel()
    {
        reset();
    }

    void reset()
    {
        mEmojiType = EEmojiType::Char;
        mEmojiCharacter = ":(";

        mMainContent = "Your PC ran into a problem and needs to restart. We're just collecting some error info, and then we'll restart for you.";

        mCttHint = "For more information about this issue and possible fixes, visit https://www.windows.com/stopcode";
        mCttInfo = "If you call a support person, give them this info: Stop code: CRITICAL_PROCESS_DIED";
        mBgColor = QColor(0, 120, 215);

        mHotKey = "ctrl+d";
        progressTime = 0;
    }
};

class AppInfo
{
public:
    static AppInfo *instance();
    AppInfoModel& getModel(){return model;};

    void save();
    void reset();
private:
    AppInfo();
    void init();
    void loadConfig();
private:
    const QString               iniPath;
    AppInfoModel                model;
};

#endif // APPINFO_H
