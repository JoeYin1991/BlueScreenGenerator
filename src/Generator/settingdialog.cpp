#include "settingdialog.h"
#include "../Common/appinfo.h"
#include "onekeysequenceedit.h"
#include "../3rdparty/get-exe-icon/get-exe-icon.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <QFontDatabase>
#include <QRadioButton>
#include <QLineEdit>
#include <QButtonGroup>
#include <QTextEdit>
#include <QScrollArea>
#include <QLabel>
#include <QToolButton>
#include <QColorDialog>
#include <QKeySequenceEdit>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QComboBox>
#include <QEvent>
#include <QDir>
#include <QtWin>
#include <QMessageBox>
#include <QListView>

const QString ICON_NAME = "icon.ico";
const QString FOLDER_NAME = "tools/bluescreen";

const QMap<EExecCmdType, QString> cmdNameMap = {
    {EExecCmdType::NoCmd, QObject::tr("None")},
    {EExecCmdType::LockScreen, QObject::tr("Lock Screen")},
    {EExecCmdType::Logoff, QObject::tr("Log Off")},
    {EExecCmdType::Shutdown, QObject::tr("Shutdown")},
    {EExecCmdType::Reboot, QObject::tr("Reboot")},
    {EExecCmdType::Sleep, QObject::tr("Sleep")},
    {EExecCmdType::Custom, QObject::tr("Custom")}
};

const QMap<EExecCmdType, QString> cmdMap = {
    {EExecCmdType::NoCmd, ""},
    {EExecCmdType::LockScreen, "rundll32.exe user32.dll,LockWorkStation"},
    {EExecCmdType::Logoff, "logoff"},
    {EExecCmdType::Shutdown, "shutdown /s /f /t 0"},
    {EExecCmdType::Reboot, "shutdown /r /f /t 0"},
    {EExecCmdType::Sleep, "rundll32.exe powrprof.dll,SetSuspendState 0,1,0"},
    {EExecCmdType::Custom, ""}
};

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent)
    , tempIcoPath(QCoreApplication::applicationDirPath()+"/tempico.ico")
    , blueScreenIcoPath(QCoreApplication::applicationDirPath()+"/"+FOLDER_NAME+"/"+ICON_NAME)
{
    initUi();
    connectSignals();
    initData();

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
    this->resize({800, 600});
    this->setMinimumWidth(700);
}

SettingDialog::~SettingDialog()
{}

void SettingDialog::initUi()
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(vLayout);

    mScrollArea = new QScrollArea(this);
    vLayout->addWidget(mScrollArea);
    mScrollArea->setWidgetResizable(true);

    QWidget *mainWgt = new QWidget(mScrollArea);
    mScrollArea->setWidget(mainWgt);

    mMainLayout = new QVBoxLayout;
    mMainLayout->setContentsMargins(30, 30, 30, 30);
    mMainLayout->setSpacing(5);
    mainWgt->setLayout(mMainLayout);

    initEmojiWgtUi();
    initContentWgtUi();
    initContactWgtUi();
    initLogoWgtUi();
    initSettingWgtUi();
    initBuildWgtUi();
}

void SettingDialog::initData()
{
    const AppInfoModel &model = AppInfo::instance()->getModel();
    if (model.mEmojiType == EEmojiType::Char) {
        mEmojiCharRB->click();
        mEmojiCharLE->setText(model.mEmojiCharacter);
    } else {
        mEmojiImgRB->click();
    }

    mContentTE->setText(model.mMainContent);

    mCttHintTE->setText(model.mCttHint);
    mCttInfoTE->setText(model.mCttInfo);

    mBgScanBtn->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3)")
            .arg(model.mBgColor.red())
            .arg(model.mBgColor.green())
            .arg(model.mBgColor.blue()));

    mFontScanBtn->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3)")
            .arg(model.mFontColor.red())
            .arg(model.mFontColor.green())
            .arg(model.mFontColor.blue()));


    mHotKeyKSE->setKeySequence(model.mHotKey);
    mProgressLE->setText(QString::number(model.progressTime));
    mCmdTE->setText(model.cmd);
    for(auto it = cmdMap.begin(); it != cmdMap.end(); it++)
    {
        if (it.value() == model.cmd)
        {
            for(int i = 0; i < mCmdCbb->count(); i++)
            {
                if (it.key() == (EExecCmdType)mCmdCbb->itemData(i).toUInt())
                {
                    if (mCmdCbb->currentIndex() != i) {
                        mCmdCbb->setCurrentIndex(i);
                    } else {
                        slotCmdTypeIndexChanged(i);
                    }
                    break;
                }
            }
            break;
        }
    }
}

void SettingDialog::initEmojiWgtUi()
{
    mEmojiWgt = new QWidget(this);
    mEmojiWgt->setObjectName(QStringLiteral("GroupWidget"));
    mEmojiWgt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mMainLayout->addWidget(mEmojiWgt);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    mEmojiWgt->setLayout(vLayout);

    QWidget *headerWgt = initGroupItemWidget(mEmojiWgt, EGroupItemType::FirstItem);
    initGroupItemHeaderUi(headerWgt,
                          tr("Emoji Settings"),
                          tr("Set the emoji with special characters or an image."));

    QWidget *charWgt = initGroupItemWidget(mEmojiWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    QHBoxLayout *charWgtLyt = qobject_cast<QHBoxLayout*>(charWgt->layout());
    mEmojiCharRB = new QRadioButton(charWgt);
    mEmojiCharRB->setText(tr("Use characters: "));
    charWgtLyt->addWidget(mEmojiCharRB);
    charWgtLyt->addSpacing(20);

    mEmojiCharLE = new QLineEdit(charWgt);
    mEmojiCharLE->setText("");
    charWgtLyt->addWidget(mEmojiCharLE);

    QWidget *imgWgt = initGroupItemWidget(mEmojiWgt, EGroupItemType::LastItem, ELayoutType::HBox);
    QHBoxLayout *imgWgtLyt = qobject_cast<QHBoxLayout*>(imgWgt->layout());
    mEmojiImgRB = new QRadioButton(imgWgt);
    mEmojiImgRB->setText(tr("Use image: "));
    imgWgtLyt->addWidget(mEmojiImgRB);
    imgWgtLyt->addSpacing(20);

    mEmojiImgLE = new QLineEdit(imgWgt);
    mEmojiImgLE->setReadOnly(true);
    mEmojiImgLE->setDisabled(true);
    imgWgtLyt->addWidget(mEmojiImgLE);
    imgWgtLyt->addSpacing(5);

    mEmojiImgScanBtn = new QToolButton(imgWgt);
    mEmojiImgScanBtn->setObjectName(QStringLiteral("BrowserIconButton"));
    imgWgtLyt->addWidget(mEmojiImgScanBtn);

    QButtonGroup *radioGroup = new QButtonGroup(mEmojiWgt);
    radioGroup->addButton(mEmojiCharRB);
    radioGroup->addButton(mEmojiImgRB);
    radioGroup->setExclusive(true);
}

void SettingDialog::initContentWgtUi()
{
    mContentWgt = new QWidget(this);
    mContentWgt->setObjectName(QStringLiteral("GroupWidget"));
    mContentWgt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mMainLayout->addWidget(mContentWgt);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    mContentWgt->setLayout(vLayout);

    QWidget *headerWgt = initGroupItemWidget(mContentWgt, EGroupItemType::FirstItem);
    initGroupItemHeaderUi(headerWgt,
                          tr("Main Content Settings"),
                          tr("Set the main content which will be displayed in the center of blue screen."));

    QWidget *teWgt = initGroupItemWidget(mContentWgt, EGroupItemType::LastItem);
    teWgt->setStyleSheet("#GroupLastItemWidget{min-height:80px;max-height:80px;}");
    QVBoxLayout *teWgtLyt = qobject_cast<QVBoxLayout*>(teWgt->layout());

    mContentTE = new QTextEdit(teWgt);
    teWgtLyt->addWidget(mContentTE);
}

void SettingDialog::initContactWgtUi()
{
    mContactWgt = new QWidget(this);
    mContactWgt->setObjectName(QStringLiteral("GroupWidget"));
    mContactWgt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mMainLayout->addWidget(mContactWgt);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    mContactWgt->setLayout(vLayout);

    QWidget *headerWgt = initGroupItemWidget(mContactWgt, EGroupItemType::FirstItem);
    initGroupItemHeaderUi(headerWgt,
                          tr("Contact Settings"),
                          tr("Set the contact information that user can contact you."));

    QWidget *qrcodeWgt = initGroupItemWidget(mContactWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initContactQRCodeWgtUi(qrcodeWgt);

    QWidget *hintWgt = initGroupItemWidget(mContactWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    hintWgt->setStyleSheet("#GroupMiddleItemWidget{min-height:80px;max-height:80px;}");
    initContactHintWgtUi(hintWgt);

    QWidget *infoWgt = initGroupItemWidget(mContactWgt, EGroupItemType::LastItem, ELayoutType::HBox);
    infoWgt->setStyleSheet("#GroupLastItemWidget{min-height:80px;max-height:80px;}");
    initContactInfoWgtUi(infoWgt);
}

void SettingDialog::initContactQRCodeWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mQRCodeLbl = new QLabel(parent);
    mQRCodeLbl->setText(tr("QRCode image path:"));
    hLayout->addWidget(mQRCodeLbl);
    hLayout->addSpacing(20);

    mQRCodeLE = new QLineEdit(parent);
    mQRCodeLE->setReadOnly(true);
    mQRCodeLE->setPlaceholderText(tr("Use default QRCode"));
    hLayout->addWidget(mQRCodeLE);
    hLayout->addSpacing(5);

    mQRCodeScanBtn = new QToolButton(parent);
    mQRCodeScanBtn->setObjectName(QStringLiteral("BrowserIconButton"));
    hLayout->addWidget(mQRCodeScanBtn);
}

void SettingDialog::initContactHintWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mCttHintLbl = new QLabel(parent);
    mCttHintLbl->setText(tr("Hint:"));
    hLayout->addWidget(mCttHintLbl);
    hLayout->addSpacing(20);

    mCttHintTE = new QTextEdit(parent);
    hLayout->addWidget(mCttHintTE);
}

void SettingDialog::initContactInfoWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mCttInfoLbl = new QLabel(parent);
    mCttInfoLbl->setText(tr("Information:"));
    hLayout->addWidget(mCttInfoLbl);
    hLayout->addSpacing(20);

    mCttInfoTE = new QTextEdit(parent);
    hLayout->addWidget(mCttInfoTE);
}

void SettingDialog::initLogoWgtUi()
{
    mLogoWgt = new QWidget(this);
    mLogoWgt->setObjectName(QStringLiteral("GroupWidget"));
    mLogoWgt->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mMainLayout->addWidget(mLogoWgt);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    mLogoWgt->setLayout(vLayout);

    QWidget *headerWgt = initGroupItemWidget(mLogoWgt, EGroupItemType::FirstItem);
    initGroupItemHeaderUi(headerWgt,
                          tr("Icon Settings"),
                          tr("Set the icon image."));

    QButtonGroup *radioGroup = new QButtonGroup(mLogoWgt);
    radioGroup->setExclusive(true);

    QWidget *exePathWgt = initGroupItemWidget(mLogoWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initLogoExePathWgtUi(exePathWgt, radioGroup);

    QWidget *exePidWgt = initGroupItemWidget(mLogoWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initLogoExePidWgtUi(exePidWgt, radioGroup);

    QWidget *imgPathWgt = initGroupItemWidget(mLogoWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initLogoImgWgtUi(imgPathWgt, radioGroup);

    QWidget *prevWgt = initGroupItemWidget(mLogoWgt, EGroupItemType::LastItem, ELayoutType::HBox);
    prevWgt->setStyleSheet("#GroupLastItemWidget{min-height:80px;max-height:80px;}");
    initLogoPrevUi(prevWgt);
}

void SettingDialog::initLogoExePathWgtUi(QWidget *parent, QButtonGroup *radioGroup)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mLogoFromExePathRB = new QRadioButton(parent);
    mLogoFromExePathRB->setText(tr("Extract ico file from exe:"));
    radioGroup->addButton(mLogoFromExePathRB);
    hLayout->addWidget(mLogoFromExePathRB);
    hLayout->addSpacing(20);

    mLogoExePathLE = new QLineEdit(parent);
    mLogoExePathLE->setReadOnly(true);
    mLogoExePathLE->setDisabled(true);
    hLayout->addWidget(mLogoExePathLE);
    hLayout->addSpacing(5);

    mLogoExePathScanBtn = new QToolButton(parent);
    mLogoExePathScanBtn->setObjectName(QStringLiteral("BrowserIconButton"));
    hLayout->addWidget(mLogoExePathScanBtn);
}

void SettingDialog::initLogoExePidWgtUi(QWidget *parent, QButtonGroup *radioGroup)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mLogoFromExePidRB = new QRadioButton(parent);
    mLogoFromExePidRB->setText(tr("Extract ico file from process pid:"));
    radioGroup->addButton(mLogoFromExePidRB);
    hLayout->addWidget(mLogoFromExePidRB);
    hLayout->addSpacing(20);

    mLogoExePidLE = new QLineEdit(parent);
    mLogoExePidLE->setDisabled(true);
    hLayout->addWidget(mLogoExePidLE);
}

void SettingDialog::initLogoImgWgtUi(QWidget *parent, QButtonGroup *radioGroup)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mLogoFromImgRB = new QRadioButton(parent);
    mLogoFromImgRB->setText(tr("Create ico file from image:"));
    radioGroup->addButton(mLogoFromImgRB);
    hLayout->addWidget(mLogoFromImgRB);
    hLayout->addSpacing(20);

    mLogoImgPathLE = new QLineEdit(parent);
    mLogoImgPathLE->setReadOnly(true);
    mLogoImgPathLE->setDisabled(true);
    hLayout->addWidget(mLogoImgPathLE);
    hLayout->addSpacing(5);

    mLogoImgPathScanBtn = new QToolButton(parent);
    mLogoImgPathScanBtn->setObjectName(QStringLiteral("BrowserIconButton"));
    hLayout->addWidget(mLogoImgPathScanBtn);
}

void SettingDialog::initLogoPrevUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mLogoIcoLbl = new QLabel(parent);
    mLogoIcoLbl->setText(tr("Ico Preview:"));
    hLayout->addWidget(mLogoIcoLbl);
    hLayout->addSpacing(20);

    mLogoIcoPrevLbl = new QLabel(parent);
    mLogoIcoPrevLbl->setObjectName(QStringLiteral("IcoPreviewLabel"));
    mLogoIcoPrevLbl->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(mLogoIcoPrevLbl);
    hLayout->addStretch();

    parent->setMinimumHeight(90);
}

void SettingDialog::initSettingWgtUi()
{
    mSettingWgt = new QWidget(this);
    mSettingWgt->setObjectName(QStringLiteral("GroupWidget"));
    mMainLayout->addWidget(mSettingWgt);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setSpacing(0);
    mSettingWgt->setLayout(vLayout);

    QWidget *headerWgt = initGroupItemWidget(mSettingWgt, EGroupItemType::FirstItem);
    initGroupItemHeaderUi(headerWgt,
                          tr("Other Settings"),
                          tr("Other settings such as background color, hot key, progress, etc."));

    QWidget *bgWgt = initGroupItemWidget(mSettingWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initSettingBgWgtUi(bgWgt);

    QWidget *hotkeyWgt = initGroupItemWidget(mSettingWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initSettingHotKeyWgtUi(hotkeyWgt);

    QWidget *progressWgt = initGroupItemWidget(mSettingWgt, EGroupItemType::MiddleItem, ELayoutType::HBox);
    initSettingProgressWgtUi(progressWgt);

    mCmdWgt = initGroupItemWidget(mSettingWgt, EGroupItemType::LastItem, ELayoutType::VBox);
    mCmdWgt->setStyleSheet("#GroupLastItemWidget{min-height:120px;max-height:120px;}");
    initSettingCmdWgtUi(mCmdWgt);
}

void SettingDialog::initSettingBgWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mBgLbl = new QLabel(parent);
    mBgLbl->setText(tr("Background color:"));
    hLayout->addWidget(mBgLbl);
    hLayout->addSpacing(10);

    mBgScanBtn = new QToolButton(parent);
    mBgScanBtn->setObjectName(QStringLiteral("ColorSelectButton"));
    hLayout->addWidget(mBgScanBtn);
    hLayout->addSpacing(40);

    mFontLbl = new QLabel(parent);
    mFontLbl->setText(tr("Font color:"));
    hLayout->addWidget(mFontLbl);
    hLayout->addSpacing(10);

    mFontScanBtn = new QToolButton(parent);
    mFontScanBtn->setObjectName(QStringLiteral("ColorSelectButton"));
    hLayout->addWidget(mFontScanBtn);
    hLayout->addStretch();
}

void SettingDialog::initSettingProgressWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mProgressLbl = new QLabel(parent);
    mProgressLbl->setText(tr("Progress total time(unit: s):"));
    hLayout->addWidget(mProgressLbl);
    hLayout->addSpacing(20);

    mProgressLE = new QLineEdit(parent);
    mProgressLE->setValidator(new QIntValidator(0, 2000000000));
    hLayout->addWidget(mProgressLE);
}

void SettingDialog::initSettingHotKeyWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mHotKeyLbl = new QLabel(parent);
    mHotKeyLbl->setText(tr("Hot key to quit blue screen:"));
    hLayout->addWidget(mHotKeyLbl);
    hLayout->addSpacing(20);

    mHotKeyKSE = new OneKeySequenceEdit(parent);
    hLayout->addWidget(mHotKeyKSE);
    hLayout->addStretch();
}

void SettingDialog::initSettingCmdWgtUi(QWidget *parent)
{
    QVBoxLayout *vLayout = qobject_cast<QVBoxLayout*>(parent->layout());
    vLayout->setSpacing(10);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(10);
    vLayout->addLayout(hLayout);
    mCmdLbl = new QLabel(parent);
    mCmdLbl->setText(tr("Execute when progress up to 100%:"));
    hLayout->addWidget(mCmdLbl);

    mCmdCbb = new QComboBox(parent);
    mCmdCbb->setView(new QListView);
    hLayout->addWidget(mCmdCbb);
    for(auto it = cmdNameMap.begin(); it != cmdNameMap.end(); it++) {
        mCmdCbb->addItem(it.value(), QVariant::fromValue(it.key()));
    }
    mCmdTE = new QTextEdit(parent);
    mCmdTE->setPlaceholderText(tr("Input custom cmd command."));
    vLayout->addWidget(mCmdTE);
}

void SettingDialog::initGroupItemHeaderUi(QWidget *parentWgt,
                                          const QString &titleText,
                                          const QString &descText)
{
    parentWgt->setObjectName(QStringLiteral("GroupWidgetHeader"));
    QVBoxLayout *vLayout = qobject_cast<QVBoxLayout*>(parentWgt->layout());
    vLayout->setContentsMargins(20, 15, 20, 15);
    QLabel *titleLbl = new QLabel(parentWgt);
    titleLbl->setObjectName(QStringLiteral("GroupWidgetHeaderTitle"));
    titleLbl->setText(titleText);
    vLayout->addWidget(titleLbl);

    QLabel *descLbl = new QLabel(parentWgt);
    descLbl->setObjectName(QStringLiteral("GroupWidgetHeaderDescription"));
    descLbl->setText(descText);
    vLayout->addWidget(descLbl);
}

QWidget *SettingDialog::initGroupItemWidget(QWidget *parent,
                                            EGroupItemType itemType /* = EGroupItemType::MiddleItem*/,
                                            ELayoutType layoutType /* = ELayoutType::VBox*/)
{
    QWidget *wgt = new QWidget(parent);
    switch (itemType) {
    case EGroupItemType::FirstItem:
        wgt->setObjectName(QStringLiteral("GroupFirstItemWidget"));
        break;
    case EGroupItemType::MiddleItem:
        wgt->setObjectName(QStringLiteral("GroupMiddleItemWidget"));
        break;
    case EGroupItemType::LastItem:
        wgt->setObjectName(QStringLiteral("GroupLastItemWidget"));
        break;
    default:
        break;
    }

    parent->layout()->addWidget(wgt);

    if (layoutType == ELayoutType::HBox)
    {
        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->setContentsMargins(35, 10, 35, 10);
        hLayout->setSpacing(0);
        wgt->setLayout(hLayout);
    }
    else if (layoutType == ELayoutType::VBox)
    {
        QVBoxLayout *vLayout = new QVBoxLayout;
        vLayout->setContentsMargins(35, 10, 35, 10);
        vLayout->setSpacing(0);
        wgt->setLayout(vLayout);
    }
    return wgt;
}

void SettingDialog::initBuildWgtUi()
{
    mBuildWgt = new QWidget(this);
    mBuildWgt->setObjectName(QStringLiteral("GroupWidget"));
    mMainLayout->addWidget(mBuildWgt);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 20, 0, 20);
    hLayout->setSpacing(0);
    mBuildWgt->setLayout(hLayout);
    hLayout->addStretch();

    mBuildBtn = new QToolButton(mBuildWgt);
    mBuildBtn->setObjectName(QStringLiteral("BuildButton"));
    mBuildBtn->setText(tr("Generate Blue Screen Exe"));
    hLayout->addWidget(mBuildBtn);
    hLayout->addSpacing(20);

    mResetBtn = new QToolButton(mBuildWgt);
    mResetBtn->setObjectName(QStringLiteral("BuildButton"));
    mResetBtn->setText(tr("Reset to Default Settings"));
    hLayout->addWidget(mResetBtn);
    hLayout->addStretch();
}

void SettingDialog::connectSignals()
{
    connect(mEmojiCharRB, &QRadioButton::clicked, this, &SettingDialog::slotEmojiRadioBtnClicked);
    connect(mEmojiImgRB, &QRadioButton::clicked, this, &SettingDialog::slotEmojiRadioBtnClicked);

    connect(mLogoFromExePathRB, &QRadioButton::clicked, this, &SettingDialog::slotIconRadioBtnClicked);
    connect(mLogoFromExePidRB, &QRadioButton::clicked, this, &SettingDialog::slotIconRadioBtnClicked);
    connect(mLogoFromImgRB, &QRadioButton::clicked, this, &SettingDialog::slotIconRadioBtnClicked);
    connect(mLogoExePidLE, &QLineEdit::editingFinished, this, &SettingDialog::slotIconFromPidLEEditFinished);
    connect(mLogoExePathScanBtn, &QToolButton::clicked, this, &SettingDialog::slotIconFromExeScanBtnClicked);
    connect(mLogoExePathLE, &QLineEdit::textChanged, this, &SettingDialog::slotIconFromExeTextChanged);
    connect(mLogoImgPathScanBtn, &QToolButton::clicked, this, &SettingDialog::slotIconFromImgScanBtnClicked);
    connect(mLogoImgPathLE, &QLineEdit::textChanged, this, &SettingDialog::slotIconFromImgTextChanged);

    connect(mBgScanBtn, &QToolButton::clicked, this, &SettingDialog::slotBgScanBtnClicked);
    connect(mFontScanBtn, &QToolButton::clicked, this, &SettingDialog::slotFontScanBtnClicked);
    connect(mBuildBtn, &QToolButton::clicked, this, &SettingDialog::slotBuildBtnClicked);
    connect(mResetBtn, &QToolButton::clicked, this, &SettingDialog::slotResetBtnClicked);
    connect(mCmdCbb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SettingDialog::slotCmdTypeIndexChanged);
}

void SettingDialog::updateIcoPreview(PBYTE buffer, DWORD outLen)
{
    if (buffer != nullptr && outLen != 0)
    {
        QFile outFile(tempIcoPath);
        if (!outFile.open(QIODevice::WriteOnly)) {
            return;
        }
        outFile.write((char*)buffer, outLen);
        outFile.close();
        updateIcoPreview();
    }
    else
    {
        mLogoIcoPrevLbl->clear();
    }
}

void SettingDialog::updateIcoPreview()
{
    int cx = GetSystemMetrics(SM_CXICON), cy = GetSystemMetrics(SM_CYICON);
    std::wstring stdIconPath = tempIcoPath.toStdWString();
    HICON hIcon = (HICON)LoadImage(0, stdIconPath.c_str(), IMAGE_ICON, cx, cy, LR_LOADFROMFILE);
    QPixmap pixmap = QtWin::fromHICON(hIcon);
    if (pixmap.isNull()) {
        mLogoIcoPrevLbl->clear();
    } else {
        mLogoIcoPrevLbl->setPixmap(pixmap);
    }
}

bool SettingDialog::eventFilter(QObject *obj, QEvent *event)
{
    return QDialog::eventFilter(obj, event);
}

void SettingDialog::slotEmojiRadioBtnClicked()
{
    if (mEmojiCharRB->isChecked())
    {
        mEmojiCharLE->setEnabled(true);
        mEmojiImgLE->setEnabled(false);
        mEmojiImgScanBtn->setEnabled(false);
    }
    else
    {
        mEmojiCharLE->setEnabled(false);
        mEmojiImgLE->setEnabled(true);
        mEmojiImgScanBtn->setEnabled(true);
    }
}

void SettingDialog::slotIconRadioBtnClicked()
{
    if (mLogoFromExePathRB->isChecked())
    {
        mLogoExePathLE->setEnabled(true);
        mLogoExePathScanBtn->setEnabled(true);
        mLogoExePidLE->setEnabled(false);
        mLogoImgPathLE->setEnabled(false);
        mLogoImgPathScanBtn->setEnabled(false);
        slotIconFromExeTextChanged();
    }
    else if (mLogoFromExePidRB->isChecked())
    {
        mLogoExePathLE->setEnabled(false);
        mLogoExePathScanBtn->setEnabled(false);
        mLogoExePidLE->setEnabled(true);
        mLogoImgPathLE->setEnabled(false);
        mLogoImgPathScanBtn->setEnabled(false);
        slotIconFromPidLEEditFinished();
    }
    else if (mLogoFromImgRB->isChecked())
    {
        mLogoExePathLE->setEnabled(false);
        mLogoExePathScanBtn->setEnabled(false);
        mLogoExePidLE->setEnabled(false);
        mLogoImgPathLE->setEnabled(true);
        mLogoImgPathScanBtn->setEnabled(true);
        slotIconFromImgTextChanged();
    }
}

void SettingDialog::slotIconFromPidLEEditFinished()
{
    if (mLogoExePidLE->text().isEmpty()) {
        return;
    }
    static int pid = 0;
    int newPid = mLogoExePidLE->text().toUInt();
    if (pid == newPid) {
        return;
    }
    pid = newPid;
    PBYTE buffer = nullptr;
    DWORD outLen = 0;
    buffer = get_exe_icon_from_pid(pid, TRUE, &outLen);
    updateIcoPreview(buffer, outLen);
}

void SettingDialog::slotIconFromExeScanBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Extract icon from exe"),
                                                    "/home",
                                                    tr("Exe (*.exe)"));
    if (fileName.isEmpty()) {
        return;
    }
    mLogoExePathLE->setText(fileName);
}

void SettingDialog::slotIconFromExeTextChanged()
{
    if (mLogoExePathLE->text().isEmpty()) {
        return;
    }
    PBYTE buffer = nullptr;
    DWORD outLen = 0;
    std::string exePath = mLogoExePathLE->text().toStdString();
    buffer = get_exe_icon_from_file_utf8(exePath.c_str(), TRUE, &outLen);
    updateIcoPreview(buffer, outLen);
}

void SettingDialog::slotIconFromImgScanBtnClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select icon file"),
                                                    "/home",
                                                    tr("ICO (*.ico)"));
    if (fileName.isEmpty()) {
        return;
    }
    mLogoImgPathLE->setText(fileName);
}

void SettingDialog::slotIconFromImgTextChanged()
{
    QString imgPath = mLogoImgPathLE->text();
    if (imgPath.isEmpty()) {
        return;
    }
    if (imgPath != tempIcoPath)
    {
        QFile in(imgPath);
        if (!in.open(QIODevice::ReadOnly)) {
            return;
        }

        QByteArray content = in.readAll();
        in.close();
        QFile out(tempIcoPath);
        if (!out.open(QIODevice::WriteOnly)) {
            return;
        }
        out.write(content);
        out.close();
    }
    updateIcoPreview();
}

void SettingDialog::slotBgScanBtnClicked()
{
    QColor color = QColorDialog::getColor(
        AppInfo::instance()->getModel().mBgColor,
        this,
        tr("Select Background")
        );
    if (!color.isValid()) {
        return;
    }

    AppInfo::instance()->getModel().mBgColor = color;

    mBgScanBtn->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3)")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue()));
}

void SettingDialog::slotFontScanBtnClicked()
{
    QColor color = QColorDialog::getColor(
        AppInfo::instance()->getModel().mBgColor,
        this,
        tr("Select Background")
        );
    if (!color.isValid()) {
        return;
    }

    AppInfo::instance()->getModel().mFontColor = color;
    mFontScanBtn->setStyleSheet(
        QString("background-color: rgb(%1,%2,%3)")
            .arg(color.red())
            .arg(color.green())
            .arg(color.blue()));
}

void SettingDialog::slotBuildBtnClicked()
{
    AppInfoModel &model = AppInfo::instance()->getModel();
    if (mEmojiCharRB->isChecked())
    {
        model.mEmojiType = EEmojiType::Char;
        model.mEmojiCharacter = mEmojiCharLE->text();
        model.mEmojiImgName = "";
    }
    else
    {
        model.mEmojiType = EEmojiType::Img;
        model.mEmojiCharacter = "";
        model.mEmojiImgName = mEmojiImgLE->text();
    }

    model.mMainContent = mContentTE->toPlainText();
    model.mCttHint = mCttHintTE->toPlainText();
    model.mCttInfo = mCttInfoTE->toPlainText();

    model.progressTime = mProgressLE->text().toUInt();
    model.mHotKey = mHotKeyKSE->keySequence().toString();

    EExecCmdType type = (EExecCmdType)mCmdCbb->currentData().toUInt();
    if (type == EExecCmdType::Custom) {
        model.cmd = mCmdTE->toPlainText();
    } else {
        model.cmd = cmdMap[type];
    }

    QString iconPath = tempIcoPath;
    if (!QFileInfo::exists(iconPath)) {
        iconPath = ":/img/icon.ico";
    }

    QString iconFolder = QFileInfo(blueScreenIcoPath).absolutePath();
    QDir dir;
    if (!dir.exists(iconFolder)) {
        dir.mkpath(iconFolder);
    }
    QFile file(iconPath);
    file.copy(blueScreenIcoPath);
    AppInfo::instance()->save();
}

void SettingDialog::slotResetBtnClicked()
{
    AppInfo::instance()->reset();
    initData();
}

void SettingDialog::slotCmdTypeIndexChanged(int index)
{
    Q_UNUSED(index);
    EExecCmdType cmdType = (EExecCmdType)mCmdCbb->currentData().toInt();
    if (cmdType == EExecCmdType::Custom)
    {
        mCmdTE->setVisible(true);
        mCmdWgt->setStyleSheet("#GroupLastItemWidget{min-height:120px;max-height:120px;}");
    }
    else
    {
        mCmdTE->setVisible(false);
        mCmdWgt->setStyleSheet("#GroupLastItemWidget{min-height:60px;max-height:60px;}");
    }
}
