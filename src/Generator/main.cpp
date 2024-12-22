#include "settingdialog.h"
#include <QApplication>
#include <QFile>
#include <QTranslator>

void loadGlobalQss()
{
    QFile file(QStringLiteral(":/qss/global"));
    if (!file.open(QIODevice::ReadOnly))
    {
        return;
    }
    QString content = QLatin1String(file.readAll());
    qApp->setStyleSheet(content);
    file.close();
}

void loadLanugage()
{
    QTranslator *trans = new QTranslator(qApp);

    if (QString(LANGUAGE_MODE) == "ZH_CN"){
        trans->load(":/languages/zh");
    } else {
        trans->load(":/languages/en");
    }
    qApp->installTranslator(trans);
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication a(argc, argv);
    loadGlobalQss();
    loadLanugage();

    SettingDialog w;
    w.show();
    return a.exec();
}
