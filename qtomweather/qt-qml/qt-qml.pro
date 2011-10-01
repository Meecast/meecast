#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative sql xml xmlpatterns network

TARGET = omweather-qml
TEMPLATE = app


SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
    qmllayoutitem.cpp \
    dataitem.cpp \
    datamodel.cpp \
    dbusadaptor.cpp \
    dbusadaptor_applet.cpp \
    controller.cpp \
    networkingcontrol.cpp \
    selectmodel.cpp \
    updatethread.cpp \
    countrymodel.cpp \
    regionmodel.cpp \
    citymodel.cpp

HEADERS  += \
    dataqml.h \
    configqml.h \
    qmllayoutitem.h \
    dataitem.h \
    datamodel.h \
    dbusadaptor.h \
    dbusadaptor_applet.h \
    controller.h \
    networkingcontrol.h \
    selectmodel.h \
    updatethread.h \
    countrymodel.h \
    regionmodel.h \
    citymodel.h

FORMS    +=
RESOURCES += weatherlayoutitem.qrc

OTHER_FILES += \
    layoutitem.qml \
    weatherlayoutitem.qml \
    Details.qml \
    ImageButton.qml \
    TextButton.qml \
    omweather.qml \
    omweatherpanel.qml \
    main.qml \
    WeatherPage.qml \
    SettingsPage.qml \
    FullWeatherPage.qml \
    AboutPage.qml \
    StationsPage.qml \
    NewStationPage.qml \
    UnitsPage.qml \
    VisualsPage.qml \
    ListPage.qml \
    SourcePage.qml \
    CountryPage.qml \
    RegionPage.qml \
    CityPage.qml \
    IconsetPage.qml

CONFIG(localdebug):DEFINES += LOCALDEBUG



INCLUDEPATH += ../core                                                                                                        
LIBS += -L ../core ../core/libomweather-core.a  
CONFIG += qdbus
CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += sqlite3
PKGCONFIG += libcurl
target.path = /opt/com.meecast.omweather/bin
INSTALLS += target

desktop.files = meecast.desktop
desktop.path = /usr/share/applications
icon64.path = /usr/share/pixmaps
icon64.files += omweather.png
qml.files = *.qml *.js
qml.path = /opt/com.meecast.omweather/share/omweather/qml
searchicon.files += gfx/*.png
searchicon.path += /opt/com.meecast.omweather/share/omweather/qml/gfx

#INSTALLS += desktop qml icon64
INSTALLS += desktop qml searchicon

DATADIR=/opt/com.meecast.omweather/share

TRANSLIST = bg_BG \
            de_DE \
            en_GB \
            en_US \
            es_ES \
            es_MX \
            fi_FI \
            fr_FR \
            it_IT \
            lv_LV \
            pl_PL \
            ru_RU \
            sk_SK

for(language, TRANSLIST):TRANSLATIONS += po/locale/$${language}/$${language}.po
!isEmpty(TRANSLATIONS) {
    isEmpty(QMAKE_LRELEASE) {
        win32:QMAKE_LRELEASE = msgfmt.exe
        else:QMAKE_LRELEASE = msgfmt
    }
    TSQM.name = msgfmt \
        ${QMAKE_FILE_IN}
    TSQM.input = TRANSLATIONS
    TSQM.output = ${QMAKE_FILE_PATH}/omweather.mo
    TSQM.commands = $$QMAKE_LRELEASE \
        -o \
        $$TSQM.output \
        ${QMAKE_FILE_IN}
    TSQM.CONFIG = no_link
    QMAKE_EXTRA_COMPILERS += TSQM
    PRE_TARGETDEPS += compiler_TSQM_make_all
}
else:message(No translation files in project)

#install

transinstallbg.extra = cp po/locale/bg_BG/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/bg/LC_MESSAGES
transinstallbg.path = /opt/com.meecast.omweather/share/locale/bg/LC_MESSAGES
transinstallbg.files = po/locale/bg_BG/omweather.mo
INSTALLS += transinstallbg

transinstallde.extra = cp po/locale/de_DE/omweather.mo  $(INSTALL_ROOT)$$DATADIR/locale/de/LC_MESSAGES
transinstallde.path = /opt/com.meecast.omweather/share/locale/de/LC_MESSAGES
transinstallde.files = po/locale/de_DE/omweather.mo
INSTALLS += transinstallde

transinstallenbg.extra = cp po/locale/en_GB/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/en_GB/LC_MESSAGES
transinstallengb.path = /opt/com.meecast.omweather/share/locale/en_GB/LC_MESSAGES
transinstallengb.files = po/locale/en_GB/omweather.mo
INSTALLS += transinstallengb

transinstallenus.extra = cp po/locale/en_US/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/en_US/LC_MESSAGES
transinstallenus.path = /opt/com.meecast.omweather/share/locale/en_US/LC_MESSAGES
transinstallenus.files = po/locale/en_US/omweather.mo
INSTALLS += transinstallenus

transinstalleses.extra = cp po/locale/es_ES/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/es_ES/LC_MESSAGES
transinstalleses.path = /opt/com.meecast.omweather/share/locale/es_ES/LC_MESSAGES
transinstalleses.files = po/locale/es_ES/omweather.mo
INSTALLS += transinstalleses

transinstallesmx.extra = cp po/locale/es_MX/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/es_MX/LC_MESSAGES
transinstallesmx.path = /opt/com.meecast.omweather/share/locale/es_MX/LC_MESSAGES
transinstallesmx.files = po/locale/es_MX/omweather.mo
INSTALLS += transinstallesmx

transinstallfi.extra = cp po/locale/fi_FI/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/fi/LC_MESSAGES
transinstallfi.path = /opt/com.meecast.omweather/share/locale/fi/LC_MESSAGES
transinstallfi.files = po/locale/fi_FI/omweather.mo
INSTALLS += transinstallfi

transinstallfr.extra = cp po/locale/fr_FR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/fr/LC_MESSAGES
transinstallfr.path = /opt/com.meecast.omweather/share/locale/fr/LC_MESSAGES
transinstallfr.files = po/locale/fr_FR/omweather.mo
INSTALLS += transinstallfr

transinstallit.extra = cp po/locale/it_IT/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/it/LC_MESSAGES
transinstallit.path = /opt/com.meecast.omweather/share/locale/it/LC_MESSAGES
transinstallit.files = po/locale/it_IT/omweather.mo
INSTALLS += transinstallit

transinstalllv.extra = cp po/locale/lv_LV/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/lv/LC_MESSAGES
transinstalllv.path = /opt/com.meecast.omweather/share/locale/lv/LC_MESSAGES
transinstalllv.files = po/locale/lv_LV/omweather.mo
INSTALLS += transinstalllv

transinstallpl.extra = cp po/locale/pl_PL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/pl/LC_MESSAGES
transinstallpl.path = /opt/com.meecast.omweather/share/locale/pl/LC_MESSAGES
transinstallpl.files = po/locale/pl_PL/omweather.mo
INSTALLS += transinstallpl

transinstallru.extra = cp po/locale/ru_RU/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ru/LC_MESSAGES
transinstallru.path = /opt/com.meecast.omweather/share/locale/ru/LC_MESSAGES
transinstallru.files = po/locale/ru_RU/omweather.mo
INSTALLS += transinstallru

transinstallsk.extra = cp po/locale/sk_SK/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sk/LC_MESSAGES
transinstallsk.path = /opt/com.meecast.omweather/share/locale/sk/LC_MESSAGES
transinstallsk.files = po/locale/sk_SK/omweather.mo
INSTALLS += transinstallsk
