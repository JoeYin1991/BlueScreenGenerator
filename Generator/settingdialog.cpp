#include "settingdialog.h"
#include "appinfo.h"
#include "onekeysequenceedit.h"
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
#include <QEvent>

SettingDialog::SettingDialog(QWidget *parent)
    : QDialog(parent)
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

    mHotKeyKSE->setKeySequence(model.mHotKey);
    mProgressLE->setText(QString::number(model.progressTime));
    mCmdTE->setText(model.cmd);
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

    QWidget *cmdWgt = initGroupItemWidget(mSettingWgt, EGroupItemType::LastItem, ELayoutType::HBox);
    cmdWgt->setStyleSheet("#GroupLastItemWidget{min-height:80px;max-height:80px;}");
    initSettingCmdWgtUi(cmdWgt);
}

void SettingDialog::initSettingBgWgtUi(QWidget *parent)
{
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mBgLbl = new QLabel(parent);
    mBgLbl->setText(tr("Background color:"));
    hLayout->addWidget(mBgLbl);
    hLayout->addSpacing(20);

    mBgScanBtn = new QToolButton(parent);
    mBgScanBtn->setObjectName(QStringLiteral("ColorSelectButton"));
    hLayout->addWidget(mBgScanBtn);
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
    QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(parent->layout());
    mCmdLbl = new QLabel(parent);
    mCmdLbl->setText(tr("Execute when progress up to 100%:"));
    hLayout->addWidget(mCmdLbl);
    hLayout->addSpacing(20);

    mCmdTE = new QTextEdit(parent);
    hLayout->addWidget(mCmdTE);
}

void SettingDialog::initGroupItemHeaderUi(QWidget *parentWgt,
                                          const QString &titleText,
                                          const QString &descText)
{
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
    connect(mEmojiCharRB, &QRadioButton::clicked, this, &SettingDialog::slotClickEmojiRadioBtn);
    connect(mEmojiImgRB, &QRadioButton::clicked, this, &SettingDialog::slotClickEmojiRadioBtn);

    connect(mBgScanBtn, &QToolButton::clicked, this, &SettingDialog::slotClickBgScanBtn);
    connect(mBuildBtn, &QToolButton::clicked, this, &SettingDialog::slotClickBuildBtn);
    connect(mResetBtn, &QToolButton::clicked, this, &SettingDialog::slotClickResetBtn);
}

bool SettingDialog::eventFilter(QObject *obj, QEvent *event)
{
    return QDialog::eventFilter(obj, event);
}

void SettingDialog::slotClickEmojiRadioBtn()
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

void SettingDialog::slotClickBgScanBtn()
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

void SettingDialog::slotClickBuildBtn()
{
    AppInfoModel &model = AppInfo::instance()->getModel();
    if (mEmojiCharRB->isChecked())
    {
        model.mEmojiType = EEmojiType::Char;
        model.mEmojiCharacter = mEmojiCharLE->text();
        model.mEmojiImgPath = "";
    }
    else
    {
        model.mEmojiType = EEmojiType::Img;
        model.mEmojiCharacter = "";
        model.mEmojiImgPath = mEmojiImgLE->text();
    }

    model.mMainContent = mContentTE->toPlainText();
    model.mCttHint = mCttHintTE->toPlainText();
    model.mCttInfo = mCttInfoTE->toPlainText();

    model.progressTime = mProgressLE->text().toUInt();
    model.mHotKey = mHotKeyKSE->keySequence().toString();
    model.cmd = mCmdTE->toPlainText();
    AppInfo::instance()->save();
}

void SettingDialog::slotClickResetBtn()
{
    AppInfo::instance()->reset();
    initData();
}
