#ifndef DEF_H
#define DEF_H

#include <QObject>
#include <QColor>

inline const char* const KEY_EMOJI_CHAR = "emoji_char";
inline const char* const KEY_EMOJI_PATH = "emoji_path";
inline const char* const KEY_MAIN_CONTENT = "main_content";
inline const char* const KEY_CONTACT_HINT = "contact_hint";
inline const char* const KEY_CONTACT_INFO = "contact_info";
inline const char* const KEY_QRCODE = "qrcode";
inline const char* const KEY_BACKGROUND_COLOR = "background_color";
inline const char* const KEY_FONT_COLOR = "font_color";
inline const char* const KEY_PROGRESS_TIME = "progress_time";
inline const char* const KEY_HOT_KEY = "hot_key";
inline const char* const KEY_EXEC_CMD = "execute_cmd";

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
    QColor          mFontColor;
    QString         mHotKey;
    int             progressTime;
    QString         progressSuffix;
    QString         cmd;

    AppInfoModel()
    {
        reset();
    }

    void reset()
    {
        mEmojiType = EEmojiType::Char;
        mEmojiCharacter = ":(";

        mMainContent = "Your PC ran into a problem and needs to restart.\nWe're just collecting some error info, and then we'll restart for you.";

        mCttHint = "For more information about this issue and possible fixes, visit https://www.windows.com/stopcode";
        mCttInfo = "If you call a support person, give them this info: Stop code: CRITICAL_PROCESS_DIED";
        mBgColor = QColor(0, 120, 215);
        mFontColor = Qt::white;

        mHotKey = "ctrl+alt+d";
        progressTime = 10;
        progressSuffix = "complete";
        cmd = "rundll32.exe user32.dll,LockWorkStation";
    }
};


#endif // DEF_H
