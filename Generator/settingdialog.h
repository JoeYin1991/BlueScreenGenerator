#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

enum class EGroupItemType {
    FirstItem,
    MiddleItem,
    LastItem,
};

enum class ELayoutType {
    VBox,
    HBox
};

class QScrollArea;
class QVBoxLayout;
class QRadioButton;
class QLineEdit;
class QTextEdit;
class QLabel;
class QToolButton;
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

    void initSettingWgtUi();
    void initSettingBgWgtUi(QWidget *parent);
    void initSettingProgressWgtUi(QWidget *parent);
    void initSettingHotKeyWgtUi(QWidget *parent);
    void initSettingCmdWgtUi(QWidget *parent);

    void loadQss();

    void initGroupItemHeaderUi(QWidget *parentWgt,
                               const QString& titleText,
                               const QString& descText);
    QWidget *initGroupItemWidget(QWidget *parent,
                                 EGroupItemType itemType = EGroupItemType::MiddleItem,
                                 ELayoutType layoutType = ELayoutType::VBox);
private:
    QScrollArea     *mScrollArea = nullptr;
    QVBoxLayout     *mMainLayout = nullptr;

    QWidget         *mEmojiWgt = nullptr;
    QRadioButton    *mEmojiCharRB = nullptr;
    QLineEdit       *mEmojiCharLE = nullptr;
    QRadioButton    *mEmojiImgRB = nullptr;
    QLineEdit       *mEmojiImgLE = nullptr;
    QToolButton     *mEmojiImgScanBtn = nullptr;

    QWidget         *mContentWgt = nullptr;
    QTextEdit       *mContentTE = nullptr;

    QWidget         *mContactWgt = nullptr;
    QLabel          *mQRCodeLbl = nullptr;
    QLineEdit       *mQRCodeLE = nullptr;
    QToolButton     *mQRCodeScanBtn = nullptr;
    QLabel          *mCttHintLbl = nullptr;
    QTextEdit       *mCttHintTE = nullptr;
    QLabel          *mCttInfoLbl = nullptr;
    QTextEdit       *mCttInfoTE = nullptr;

    QWidget         *mSettingWgt = nullptr;
    QLabel          *mBgLbl = nullptr;
    QToolButton     *mBgScanBtn = nullptr;
    QLabel          *mProgressLbl = nullptr;
    QLineEdit       *mProgressLE = nullptr;
    QLabel          *mHotKeyLbl = nullptr;
    QToolButton     *mHotKeyBtn = nullptr;
    QLabel          *mCmdLbl = nullptr;
    QTextEdit       *mCmdTE = nullptr;
};
#endif // SETTINGDIALOG_H
