#include "bluescreen.h"
#include "../Common/appinfo.h"
#include <QSettings>
#include <QTextCodec>
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileInfo>
#include <QTimer>
#include <QKeyEvent>
#include <QProcess>

BlueScreenDlg::BlueScreenDlg(bool isMainScreen,
                             QWidget *parent)
    : QDialog(parent)
    , model(AppInfo::instance()->getModel())
    , isMainScreen(isMainScreen)
    , emojiIcoPath(QCoreApplication::applicationDirPath()+"/emoji.png")
    , qrcodeIcoPath(QCoreApplication::applicationDirPath()+"/qrcode.png")
{
    setWindowFlags(Qt::FramelessWindowHint);

    if (isMainScreen)
    {
        initUi();
        initTimer();
    }
    else
    {
        this->setStyleSheet(
            QString("background-color: rgb(%1,%2,%3)")
                .arg(model.mBgColor.red())
                .arg(model.mBgColor.green())
                .arg(model.mBgColor.blue()));
    }
}

BlueScreenDlg::~BlueScreenDlg() {}

void BlueScreenDlg::slotTimeout()
{
    ++mProgress;
    updateProgress();
    if (mProgress >= 100)
    {
        mTimer->stop();
        runCmd();
        qApp->quit();
    }
}

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
    mLayout->addSpacing(20);
    initMainContentUi();
    mLayout->addSpacing(50);
    initProgressUi();
    mLayout->addSpacing(50);
    initContactUi();
    mLayout->addStretch();
}

void BlueScreenDlg::initTimer()
{
    mTimer = new QTimer(this);
    connect(mTimer, &QTimer::timeout, this, &BlueScreenDlg::slotTimeout);
    int msec = model.progressTime * 1000 / 100;
    mTimer->start(msec);
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
    else
    {
        QPixmap pixmap(emojiIcoPath);
        pixmap = pixmap.scaledToHeight(200, Qt::SmoothTransformation);
        emojiLbl->setPixmap(pixmap);
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
                                "color: rgb(%1,%2,%3);"
                                      ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    mLayout->addWidget(contentLbl);
}

void BlueScreenDlg::initProgressUi()
{
    mProgressLbl = new QLabel(this);
    mProgressLbl->setStyleSheet(QString("font-family: 'Microsoft YaHei UI';"
                                  "font-size: 30px;"
                                  "font-weight: 200;"
                                  "color: rgb(%1,%2,%3);"
                                  ).arg(model.mFontColor.red()).arg(model.mFontColor.green()).arg(model.mFontColor.blue()));
    mLayout->addWidget(mProgressLbl);
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

    QString qrCodePath = qrcodeIcoPath;
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
    mProgressLbl->setText(QString("%1% %2").arg(mProgress).arg(model.progressSuffix));
}

void BlueScreenDlg::runCmd()
{
    if (model.cmd.isEmpty()) {
        return;
    }

    QProcess proc;
    proc.startDetached("cmd.exe", {"/C", model.cmd});
}
