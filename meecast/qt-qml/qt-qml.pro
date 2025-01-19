#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT += sql xml xmlpatterns network positioning widgets qml quick quickcontrols2

TARGET = omweather-qml
TEMPLATE = app




exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):contains(MEEGO_EDITION,harmattan): {
    DEFINES += MEEGO_EDITION_HARMATTAN
}

SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
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
    citymodel.cpp \
    gpsposition.cpp

    # qmllayoutitem.cpp \
HEADERS  += \
    dataqml.h \
    configqml.h \
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
    citymodel.h \
    gpsposition.h

    # qmllayoutitem.h \

FORMS    +=
RESOURCES += resources.qrc

OTHER_FILES += \
    qml/layoutitem.qml \
    qml/weatherlayoutitem.qml \
    qml/Details.qml \
    qml/ImageButton.qml \
    qml/TextButton.qml \
    qml/omweather.qml \
    qml/omweatherpanel.qml \
    qml/main.qml \
    qml/WeatherPage.qml \
    qml/SettingsPage.qml \
    qml/FullWeatherPage.qml \
    qml/AboutPage.qml \
    qml/StationsPage.qml \
    qml/NewStationPage.qml \
    qml/UnitsPage.qml \
    qml/VisualsPage.qml \
    qml/ListPage.qml \
    qml/SourcePage.qml \
    qml/CountryPage.qml \
    qml/RegionPage.qml \
    qml/CityPage.qml \
    qml/IconsetPage.qml \
    qml/StandbyScreen.qml \
    qml/ColorCell.qml \
    qml/LanguagesPage.qml \
    qml/SearchField.qml

CONFIG(localdebug):DEFINES += LOCALDEBUG

CONFIG += mobility
MOBILITY += location
CONFIG += qdeclarative-boostable
CONFIG += meegotouch 

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic


PKGCONFIG += libxml-2.0

INCLUDEPATH += ../core                                                                                                        

LIBS += -L ../core ../core/libomweather-core.a 
LIBS += ../../omweather-fmi-fi-stations-db/tz.o ../../omweather-fmi-fi-stations-db/meego-main.o ../../omweather-fmi-fi-stations-db/jsoncpp.o ../../omweather-openweathermap-org-stations-db/meego-main.o ../../omweather-openweathermap-org-stations-db/hash.o ../../omweather-gismeteo-ru-stations-db/meego-main.o ../../omweather-gismeteo-ru-stations-db/hash.o ../../omweather-foreca-com-stations-db/meego-main.o ../../omweather-foreca-com-stations-db/hash.o ../../omweather-hko-gov-hk-stations-db/meego-main.o ../../omweather-hko-gov-hk-stations-db/hash.o ../../omweather-bom-gov-au-stations-db/meego-main.o ../../omweather-bom-gov-au-stations-db/hash.o ../../omweather-yr-no-stations-db/meego-main.o ../../omweather-yr-no-stations-db/hash.o ../../omweather-weather-com-stations-db/main.o






#LIBS += -L ../core ../core/libomweather-core.a  ../libxml2/liblibxml2.a ../../omweather-openweathermap-org-stations-db/libopenweathermaporg.a -lsailfishapp 
CONFIG += dbus
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
qml.files = pureqml/*.qml *.js
qml.path = /opt/com.meecast.omweather/share/omweather/qml
qml2.files = pureqml/pages/*
qml2.path = /opt/com.meecast.omweather/share/omweather/qml/pages

searchicon.files += gfx/*.png
searchicon.path += /opt/com.meecast.omweather/share/omweather/qml/gfx

#INSTALLS += desktop qml icon64
INSTALLS += desktop qml qml2 searchicon

DATADIR=/opt/com.meecast.omweather/share

TRANSLIST = ar_AR \
            bg_BG \
            ca_CA \
            cs_CS \
            de_DE \
            en_GB \
            en_US \
            es_ES \
            es_MX \
            fi_FI \
            fr_FR \
            da_DK \
            de_DE \
            hu_HU \
            it_IT \
            lv_LV \
            no_NO \
            nl_NL \
            pl_PL \
            pt_PT \
            ru_RU \
            sv_SV \
            sr_SR \
            sl_SL \
            sk_SK \
            sq_SQ \
            tr_TR \
            uk_UA \
            vi_VI \
            zh_TW \
            zh_HK \
	    zh_ZH

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
transinstallsq.extra = cp po/locale/sq_SQ/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sq/LC_MESSAGES
transinstallsq.path = /opt/com.meecast.omweather/share/locale/sq/LC_MESSAGES
transinstallsq.files = po/locale/sq_SQ/omweather.mo
INSTALLS += transinstallsq

transinstallar.extra = cp po/locale/ar_AR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ar/LC_MESSAGES
transinstallar.path = /opt/com.meecast.omweather/share/locale/ar/LC_MESSAGES
transinstallar.files = po/locale/ar_AR/omweather.mo
INSTALLS += transinstallar

transinstallbg.extra = cp po/locale/bg_BG/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/bg/LC_MESSAGES
transinstallbg.path = /opt/com.meecast.omweather/share/locale/bg/LC_MESSAGES
transinstallbg.files = po/locale/bg_BG/omweather.mo
INSTALLS += transinstallbg

transinstallca.extra = cp po/locale/ca_CA/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ca/LC_MESSAGES
transinstallca.path = /opt/com.meecast.omweather/share/locale/ca/LC_MESSAGES
transinstallca.files = po/locale/ca_CA/omweather.mo
INSTALLS += transinstallca

transinstallcl.extra = cp po/locale/cs_CS/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/cs/LC_MESSAGES
transinstallcl.path = /opt/com.meecast.omweather/share/locale/cs/LC_MESSAGES
transinstallcl.files = po/locale/cs_CS/omweather.mo
INSTALLS += transinstallcl

transinstallda.extra = cp po/locale/da_DK/omweather.mo  $(INSTALL_ROOT)$$DATADIR/locale/da/LC_MESSAGES
transinstallda.path = /opt/com.meecast.omweather/share/locale/da/LC_MESSAGES
transinstallda.files = po/locale/da_DK/omweather.mo
INSTALLS += transinstallda


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

transinstallhu.extra = cp po/locale/hu_HU/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/hu/LC_MESSAGES
transinstallhu.path = /opt/com.meecast.omweather/share/locale/hu/LC_MESSAGES
transinstallhu.files = po/locale/hu_HU/omweather.mo
INSTALLS += transinstallhu

transinstallit.extra = cp po/locale/it_IT/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/it/LC_MESSAGES
transinstallit.path = /opt/com.meecast.omweather/share/locale/it/LC_MESSAGES
transinstallit.files = po/locale/it_IT/omweather.mo
INSTALLS += transinstallit

transinstalllv.extra = cp po/locale/lv_LV/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/lv/LC_MESSAGES
transinstalllv.path = /opt/com.meecast.omweather/share/locale/lv/LC_MESSAGES
transinstalllv.files = po/locale/lv_LV/omweather.mo
INSTALLS += transinstalllv

transinstallno.extra = cp po/locale/no_NO/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/no/LC_MESSAGES
transinstallno.path = /opt/com.meecast.omweather/share/locale/no/LC_MESSAGES
transinstallno.files = po/locale/no_NO/omweather.mo
INSTALLS += transinstallno

transinstallnl.extra = cp po/locale/nl_NL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/nl/LC_MESSAGES
transinstallnl.path = /opt/com.meecast.omweather/share/locale/nl/LC_MESSAGES
transinstallnl.files = po/locale/nl_NL/omweather.mo
INSTALLS += transinstallnl

transinstallpl.extra = cp po/locale/pl_PL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/pl/LC_MESSAGES
transinstallpl.path = /opt/com.meecast.omweather/share/locale/pl/LC_MESSAGES
transinstallpl.files = po/locale/pl_PL/omweather.mo
INSTALLS += transinstallpl

transinstallpt.extra = cp po/locale/pt_PT/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/pt/LC_MESSAGES
transinstallpt.path = /opt/com.meecast.omweather/share/locale/pt/LC_MESSAGES
transinstallpt.files = po/locale/pt_PT/omweather.mo
INSTALLS += transinstallpt

transinstallru.extra = cp po/locale/ru_RU/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ru/LC_MESSAGES
transinstallru.path = /opt/com.meecast.omweather/share/locale/ru/LC_MESSAGES
transinstallru.files = po/locale/ru_RU/omweather.mo
INSTALLS += transinstallru

transinstallsl.extra = cp po/locale/sl_SL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sl/LC_MESSAGES
transinstallsl.path = /opt/com.meecast.omweather/share/locale/sl/LC_MESSAGES
transinstallsl.files = po/locale/sl_SL/omweather.mo
INSTALLS += transinstallsl

transinstallsk.extra = cp po/locale/sk_SK/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sk/LC_MESSAGES
transinstallsk.path = /opt/com.meecast.omweather/share/locale/sk/LC_MESSAGES
transinstallsk.files = po/locale/sk_SK/omweather.mo
INSTALLS += transinstallsk

transinstallsv.extra = cp po/locale/sv_SV/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sv/LC_MESSAGES
transinstallsv.path = /opt/com.meecast.omweather/share/locale/sv/LC_MESSAGES
transinstallsv.files = po/locale/sk_SV/omweather.mo
INSTALLS += transinstallsv

transinstallsr.extra = cp po/locale/sr_SR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sr/LC_MESSAGES
transinstallsr.path = /opt/com.meecast.omweather/share/locale/sr/LC_MESSAGES
transinstallsr.files = po/locale/sr_SR/omweather.mo
INSTALLS += transinstallsr


transinstalltr.extra = cp po/locale/tr_TR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/tr/LC_MESSAGES
transinstalltr.path = /opt/com.meecast.omweather/share/locale/tr/LC_MESSAGES
transinstalltr.files = po/locale/tr_TR/omweather.mo
INSTALLS += transinstalltr

transinstallvi.extra = cp po/locale/vi_VI/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/vi/LC_MESSAGES
transinstallvi.path = /opt/com.meecast.omweather/share/locale/vi/LC_MESSAGES
transinstallvi.files = po/locale/vi_VI/omweather.mo
INSTALLS += transinstallvi

transinstallzh.extra = cp po/locale/zh_ZH/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/zh/LC_MESSAGES
transinstallzh.path = /opt/com.meecast.omweather/share/locale/zh/LC_MESSAGES
transinstallzh.files = po/locale/zh_ZH/omweather.mo
INSTALLS += transinstallzh

transinstallzhtw.extra = cp po/locale/zh_TW/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/zh_TW/LC_MESSAGES
transinstallzhtw.path = /opt/com.meecast.omweather/share/locale/zh_TW/LC_MESSAGES
transinstallzhtw.files = po/locale/zh_TW/omweather.mo
INSTALLS += transinstallzhtw

transinstallzhhk.extra = cp po/locale/zh_HK/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/zh_HK/LC_MESSAGES
transinstallzhhk.path = /opt/com.meecast.omweather/share/locale/zh_HK/LC_MESSAGES
transinstallzhhk.files = po/locale/zh_HK/omweather.mo
INSTALLS += transinstallzhhk

transinstallukua.extra = cp po/locale/uk_UA/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/uk_UA/LC_MESSAGES
transinstallukua.path = /opt/com.meecast.omweather/share/locale/uk_UA/LC_MESSAGES
transinstallukua.files = po/locale/uk_UA/omweather.mo
INSTALLS += transinstallukua


