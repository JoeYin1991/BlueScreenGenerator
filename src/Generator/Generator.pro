QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Common/appinfo.cpp \
    3rdparty/get-exe-icon/get-exe-icon.c \
    main.cpp \
    onekeysequenceedit.cpp \
    settingdialog.cpp

HEADERS += \
    ../Common/def.h \
    ../Common/appinfo.h \
    3rdparty/get-exe-icon/get-exe-icon.h \
    onekeysequenceedit.h \
    settingdialog.h

LIBS += \
    User32.lib \
    Kernel32.lib

win32:CONFIG(debug, debug|release): {
    QMAKE_CFLAGS_DEBUG += -MTd
    QMAKE_CXXFLAGS_DEBUG += -MTd
}
else:win32:CONFIG(release, debug|release): {
    QMAKE_CFLAGS_RELEASE += -MT
    QMAKE_CXXFLAGS_RELEASE += -MT
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource/resource.qrc
