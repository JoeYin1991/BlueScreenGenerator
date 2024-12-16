#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <Windows.h>

enum class EGroupItemType {
    FirstItem,
    MiddleItem,
    LastItem,
};

enum class ELayoutType {
    VBox,
    HBox
};

enum class EExecCmdType {
    NoCmd,
    LockScreen,
    Logoff,
    Shutdown,
    Reboot,
    Sleep,
    Custom,
};
Q_DECLARE_METATYPE(EExecCmdType);

class QScrollArea;
class QVBoxLayout;
class QRadioButton;
class QLineEdit;
class QTextEdit;
class QLabel;
class QToolButton;
class QComboBox;
class OneKeySequenceEdit;
class QButtonGroup;
class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();

private:
    void initUi();
    void initData();

    void initEmojiWgtUi();
    void initContentWgtUi();

    void initContactWgtUi();
    void initContactQRCodeWgtUi(QWidget *parent);
    void initContactHintWgtUi(QWidget *parent);
    void initContactInfoWgtUi(QWidget *parent);

    void initLogoWgtUi();
    void initLogoExePathWgtUi(QWidget *parent, QButtonGroup *radioGroup);
    void initLogoExePidWgtUi(QWidget *parent, QButtonGroup *radioGroup);
    void initLogoImgWgtUi(QWidget *parent, QButtonGroup *radioGroup);
    void initLogoPrevUi(QWidget *parent);

    void initSettingWgtUi();
    void initSettingBgWgtUi(QWidget *parent);
    void initSettingProgressWgtUi(QWidget *parent);
    void initSettingHotKeyWgtUi(QWidget *parent);
    void initSettingCmdWgtUi(QWidget *parent);

    void initGroupItemHeaderUi(QWidget *parentWgt,
                               const QString& titleText,
                               const QString& descText);
    QWidget *initGroupItemWidget(QWidget *parent,
                                 EGroupItemType itemType = EGroupItemType::MiddleItem,
                                 ELayoutType layoutType = ELayoutType::VBox);

    void initBuildWgtUi();
    void connectSignals();

    void updateIcoPreview(PBYTE buffer, DWORD outLen);
    void updateIcoPreview();

    void preventComboboxScroll();

    bool buildConfig();
protected:
    bool eventFilter(QObject *o, QEvent *e) override;

private slots:
    void slotEmojiRadioBtnClicked();
    void slotEmojiImageScanBtnClicked();

    void slotIconRadioBtnClicked();
    void slotIconFromPidLEEditFinished();
    void slotIconFromExeScanBtnClicked();
    void slotIconFromExeTextChanged();
    void slotIconFromImgScanBtnClicked();
    void slotIconFromImgTextChanged();

    void slotQRCodeScanBtnClicked();
    void slotBgScanBtnClicked();
    void slotFontScanBtnClicked();
    void slotOutputScanBtnClicked();
    void slotBuildBtnClicked();
    void slotResetBtnClicked();
    void slotPreviewBtnClicked();
    void slotCmdTypeIndexChanged(int index);
private:
    QScrollArea             *mScrollArea = nullptr;
    QVBoxLayout             *mMainLayout = nullptr;

    QWidget                 *mEmojiWgt = nullptr;
    QRadioButton            *mEmojiCharRB = nullptr;
    QLineEdit               *mEmojiCharLE = nullptr;
    QRadioButton            *mEmojiImgRB = nullptr;
    QLineEdit               *mEmojiImgLE = nullptr;
    QToolButton             *mEmojiImgScanBtn = nullptr;

    QWidget                 *mContentWgt = nullptr;
    QTextEdit               *mContentTE = nullptr;

    QWidget                 *mContactWgt = nullptr;
    QLabel                  *mQRCodeLbl = nullptr;
    QLineEdit               *mQRCodeLE = nullptr;
    QToolButton             *mQRCodeScanBtn = nullptr;
    QLabel                  *mCttHintLbl = nullptr;
    QTextEdit               *mCttHintTE = nullptr;
    QLabel                  *mCttInfoLbl = nullptr;
    QTextEdit               *mCttInfoTE = nullptr;

    QWidget                 *mLogoWgt = nullptr;
    QRadioButton            *mLogoFromExePathRB = nullptr;
    QLineEdit               *mLogoExePathLE = nullptr;
    QToolButton             *mLogoExePathScanBtn = nullptr;
    QRadioButton            *mLogoFromExePidRB = nullptr;
    QLineEdit               *mLogoExePidLE = nullptr;
    QRadioButton            *mLogoFromImgRB = nullptr;
    QLineEdit               *mLogoImgPathLE = nullptr;
    QToolButton             *mLogoImgPathScanBtn = nullptr;
    QLabel                  *mLogoIcoLbl = nullptr;
    QLabel                  *mLogoIcoPrevLbl = nullptr;

    QWidget                 *mSettingWgt = nullptr;
    QLabel                  *mBgLbl = nullptr;
    QToolButton             *mBgScanBtn = nullptr;
    QLabel                  *mFontLbl = nullptr;
    QToolButton             *mFontScanBtn = nullptr;
    QLabel                  *mLogoLbl = nullptr;
    QLineEdit               *mLogoLE = nullptr;
    QToolButton             *mLogoScanBtn = nullptr;
    QLabel                  *mProgressLbl = nullptr;
    QLineEdit               *mProgressLE = nullptr;
    QLabel                  *mHotKeyLbl = nullptr;
    OneKeySequenceEdit      *mHotKeyKSE = nullptr;

    QWidget                 *mCmdWgt = nullptr;
    QLabel                  *mCmdLbl = nullptr;
    QComboBox               *mCmdCbb = nullptr;
    QTextEdit               *mCmdTE = nullptr;

    QWidget                 *mBuildWgt = nullptr;
    QLineEdit               *mOutputPathLE = nullptr;
    QToolButton             *mOutputScanBtn = nullptr;
    QToolButton             *mBuildBtn = nullptr;
    QToolButton             *mResetBtn = nullptr;
    QToolButton             *mPreviewBtn = nullptr;

    const QString           tempIcoPath;
    const QString           blueScreenIcoPath;
    const QString           emojiIcoPath;
    const QString           qrcodeIcoPath;
};
#endif // SETTINGDIALOG_H
