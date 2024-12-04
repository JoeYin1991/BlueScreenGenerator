#ifndef APPINFO_H
#define APPINFO_H

#include "../def.h"
#include <QObject>
#include <QColor>

class AppInfo
{
public:
    static AppInfo *instance();
    AppInfoModel& getModel(){return model;};

    void save();
    void reset();
private:
    AppInfo();
    void init();
    void loadConfig();
private:
    const QString               iniPath;
    AppInfoModel                model;
};

#endif // APPINFO_H
