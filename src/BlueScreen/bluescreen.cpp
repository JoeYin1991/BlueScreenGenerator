#include "bluescreen.h"
#include <QSettings>
#include <QTextCodec>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QLabel>

BlueScreenDlg::BlueScreenDlg(QWidget *parent)
    : QDialog(parent)
    , iniPath(QCoreApplication::applicationDirPath() + "/" + "config.ini")
{
    initData();
    initUi();
}

BlueScreenDlg::~BlueScreenDlg() {}

void BlueScreenDlg::initUi()
{
    mLayout = new QVBoxLayout;
    mLayout->setContentsMargins(150, 150, 550, 150);
    mLayout->setSpacing(0);
    this->setLayout(mLayout);
    this->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3)")
            .arg(model.mBgColor.red())
            .arg(model.mBgColor.green())
            .arg(model.mBgColor.blue()));

    initEmojiUi();
    mLayout->addSpacing(20);
    initMainContentUi();
    mLayout->addStretch();
}

void BlueScreenDlg::initData()
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

void BlueScreenDlg::initEmojiUi()
{
    QLabel *emojiLbl = new QLabel(this);
    if (model.mEmojiType == EEmojiType::Char)
    {
        emojiLbl->setText(model.mEmojiCharacter);
        emojiLbl->setStyleSheet(QString(
                                      "font-family: 'Microsoft YaHei UI';"
                                      "font-size: 200px;"
                                      "color: rgb(%1,%2,%3);"
                                      ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    }
    mLayout->addWidget(emojiLbl, 0, Qt::AlignLeft);
}

void BlueScreenDlg::initMainContentUi()
{
    QLabel *contentLbl = new QLabel(this);
    contentLbl->setWordWrap(true);
    contentLbl->setText(model.mMainContent);
    contentLbl->setStyleSheet(QString(
                                "font-family: 'Microsoft YaHei UI';"
                                "font-size: 40px;"
                                "color: rgb(%1,%2,%3);"
                                      ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    mLayout->addWidget(contentLbl);
}
