#include "settingdialog.h"
#include <QApplication>
#include <QFile>

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

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication a(argc, argv);
    loadGlobalQss();

    SettingDialog w;
    w.show();
    return a.exec();
}
