#ifndef DCONFMIGRATION
#define DCONFMIGRATION

#include "mdconfitem.h"
#include <QSettings>
#include <QStringList>

static void migrate_dconf()
{
    MDConfItem ready("/apps/harbour-mitakuuluu2/migrationIsDone");
    if (!ready.value(false).toBool()) {
        QSettings settings("coderus", "mitakuuluu2");
        QStringList groups = settings.childGroups();
        foreach (const QString &group, groups) {
            //printf("[%s]\n", group.toUtf8().constData());
            settings.beginGroup(group);
            QStringList keys = settings.allKeys();
            foreach (const QString &key, keys) {
                //printf("%s\n", key.toUtf8().constData());
                MDConfItem dconf(QString("/apps/harbour-mitakuuluu2/%1/%2").arg(group).arg(key));
                dconf.set(settings.value(key));
            }
            settings.endGroup();
        }
    }
}

#endif // DCONFMIGRATION 
