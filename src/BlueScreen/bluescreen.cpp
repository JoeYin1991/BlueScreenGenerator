#include "bluescreen.h"
#include <QSettings>
#include <QTextCodec>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>

BlueScreenDlg::BlueScreenDlg(QWidget *parent)
    : QDialog(parent)
    , iniPath(QCoreApplication::applicationDirPath() + "/" + "config.ini")
{
    setWindowFlags(Qt::FramelessWindowHint);

    initData();
    initUi();

}

BlueScreenDlg::~BlueScreenDlg() {}

void BlueScreenDlg::initUi()
{
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    this->setLayout(hLayout);
    hLayout->addStretch(1);

    mLayout = new QVBoxLayout;
    mLayout->setContentsMargins(0, 0, 0, 0);
    mLayout->setSpacing(0);
    hLayout->addLayout(mLayout, 5);
    hLayout->addStretch(4);

    this->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3)")
            .arg(model.mBgColor.red())
            .arg(model.mBgColor.green())
            .arg(model.mBgColor.blue()));

    mLayout->addStretch();
    initEmojiUi();
    mLayout->addSpacing(50);
    initMainContentUi();
    mLayout->addSpacing(50);
    initProgressUi();
    mLayout->addSpacing(50);
    initContactUi();
    mLayout->addStretch();
}

void BlueScreenDlg::initData()
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
        emojiLbl->setStyleSheet(QString("font-family: 'Microsoft YaHei UI';"
                                      "font-size: 150px;"
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
    contentLbl->setStyleSheet(QString("font-family: 'Microsoft YaHei UI';"
                                "font-size: 30px;"
                                "font-weight: 200;"
                                "color: rgb(%1,%2,%3);"
                                      ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    mLayout->addWidget(contentLbl);
}

void BlueScreenDlg::initProgressUi()
{
    progressLbl = new QLabel(this);
    progressLbl->setStyleSheet(QString("font-family: 'Microsoft YaHei UI';"
                                  "font-size: 30px;"
                                  "font-weight: 200;"
                                  "color: rgb(%1,%2,%3);"
                                  ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    mLayout->addWidget(progressLbl);
    updateProgress();
}

void BlueScreenDlg::initContactUi()
{
    int fixSize = 150;
    QWidget *wgt = new QWidget(this);
    wgt->setFixedHeight(fixSize);
    mLayout->addWidget(wgt);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(20);
    wgt->setLayout(hLayout);

    QLabel *qrCodeLbl = new QLabel(this);
    qrCodeLbl->setFixedSize(fixSize, fixSize);
    hLayout->addWidget(qrCodeLbl);

    QString qrCodePath = QCoreApplication::applicationDirPath() + "/img/qrcode.png";
    if (!QFileInfo::exists(qrCodePath)) {
        qrCodePath = ":/img/qrcode";
    }
    QPixmap qrCodeImg(qrCodePath);
    qrCodeImg = qrCodeImg.scaled({fixSize, fixSize}, Qt::KeepAspectRatio);
    qrCodeLbl->setPixmap(qrCodeImg);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(20);
    hLayout->addLayout(vLayout);

    QLabel *hintLbl = new QLabel(this);
    hintLbl->setText(model.mCttHint);
    hintLbl->setWordWrap(true);
    hintLbl->setStyleSheet(QString("font-family: 'Microsoft YaHei UI';"
                                   "font-size: 20px;"
                                   "font-weight: 200;"
                                   "color: rgb(%1,%2,%3);"
                                   ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    vLayout->addWidget(hintLbl);

    QLabel *infoLbl = new QLabel(this);
    infoLbl->setText(model.mCttInfo);
    infoLbl->setWordWrap(true);
    infoLbl->setStyleSheet(QString("font-family: 'Microsoft YaHei UI';"
                                   "font-size: 20px;"
                                   "font-weight: 200;"
                                   "color: rgb(%1,%2,%3);"
                                   ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    vLayout->addWidget(infoLbl);
    vLayout->addStretch();
}

void BlueScreenDlg::updateProgress()
{
    progressLbl->setText(QString("%1% %2").arg(progress).arg(model.progressSuffix));
}
