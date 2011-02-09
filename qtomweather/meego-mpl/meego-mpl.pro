#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------


TARGET = meego-panel-omweather
TEMPLATE = app


TEXTDOMAIN = "omweather"
CATALOUGE  = omweather.por


HEADERS += meego-netbookUX-omweather.h \
           meego-netbookUX-common.h

SOURCES += meego-panel-omweather.cpp \
           meego-netbookUX-omweather.cpp


FORMS    +=

OTHER_FILES += \


INCLUDEPATH += ../core
LIBS += -L ../core -lomweather-core
CONFIG = link_pkgconfig -qt
PKGCONFIG += dbus-glib-1 \
             mx-1.0 \
             mutter-plugins \
             meego-panel \
             QtCore


DATADIR=/usr/share

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
target.path = /usr/libexec
desktop.files = data/meego-panel-omweather.desktop
desktop.path = /usr/share/mutter-meego/panels
autostart.files = data/meego-panel-omweather.desktop
autostart.path = /etc/xdg
css.files = data/omweather-panel.css
css.path = /usr/share/meego-panel-omweather/theme
service.files = data/com.meego.UX.Shell.Panels.omweather.service
service.path = /usr/share/dbus-1/services
INSTALLS += target desktop autostart css service

transinstallbg.extra = cp po/locale/bg_BG/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/bg/LC_MESSAGES
transinstallbg.path = /usr/share/locale/bg/LC_MESSAGES
transinstallbg.files = po/locale/bg_BG/omweather.mo
INSTALLS += transinstallbg

transinstallde.extra = cp po/locale/de_DE/omweather.mo  $(INSTALL_ROOT)$$DATADIR/locale/de/LC_MESSAGES
transinstallde.path = /usr/share/locale/de/LC_MESSAGES
transinstallde.files = po/locale/de_DE/omweather.mo
INSTALLS += transinstallde

transinstallenbg.extra = cp po/locale/en_GB/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/en_GB/LC_MESSAGES
transinstallengb.path = /usr/share/locale/en_GB/LC_MESSAGES
transinstallengb.files = po/locale/en_GB/omweather.mo
INSTALLS += transinstallengb

transinstallenus.extra = cp po/locale/en_US/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/en_US/LC_MESSAGES
transinstallenus.path = /usr/share/locale/en_US/LC_MESSAGES
transinstallenus.files = po/locale/en_US/omweather.mo
INSTALLS += transinstallenus

transinstalleses.extra = cp po/locale/es_ES/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/es_ES/LC_MESSAGES
transinstalleses.path = /usr/share/locale/es_ES/LC_MESSAGES
transinstalleses.files = po/locale/es_ES/omweather.mo
INSTALLS += transinstalleses

transinstallesmx.extra = cp po/locale/es_MX/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/es_MX/LC_MESSAGES
transinstallesmx.path = /usr/share/locale/es_MX/LC_MESSAGES
transinstallesmx.files = po/locale/es_MX/omweather.mo
INSTALLS += transinstallesmx

transinstallfi.extra = cp po/locale/fi_FI/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/fi/LC_MESSAGES
transinstallfi.path = /usr/share/locale/fi/LC_MESSAGES
transinstallfi.files = po/locale/fi_FI/omweather.mo
INSTALLS += transinstallfi

transinstallfr.extra = cp po/locale/fr_FR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/fr/LC_MESSAGES
transinstallfr.path = /usr/share/locale/fr/LC_MESSAGES
transinstallfr.files = po/locale/fr_FR/omweather.mo
INSTALLS += transinstallfr

transinstallit.extra = cp po/locale/it_IT/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/it/LC_MESSAGES
transinstallit.path = /usr/share/locale/it/LC_MESSAGES
transinstallit.files = po/locale/it_IT/omweather.mo
INSTALLS += transinstallit

transinstalllv.extra = cp po/locale/lv_LV/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/lv/LC_MESSAGES
transinstalllv.path = /usr/share/locale/lv/LC_MESSAGES
transinstalllv.files = po/locale/lv_LV/omweather.mo
INSTALLS += transinstalllv

transinstallpl.extra = cp po/locale/pl_PL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/pl/LC_MESSAGES
transinstallpl.path = /usr/share/locale/pl/LC_MESSAGES
transinstallpl.files = po/locale/pl_PL/omweather.mo
INSTALLS += transinstallpl

transinstallru.extra = cp po/locale/ru_RU/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ru/LC_MESSAGES
transinstallru.path = /usr/share/locale/ru/LC_MESSAGES
transinstallru.files = po/locale/ru_RU/omweather.mo
INSTALLS += transinstallru

transinstallsk.extra = cp po/locale/sk_SK/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sk/LC_MESSAGES
transinstallsk.path = /usr/share/locale/sk/LC_MESSAGES
transinstallsk.files = po/locale/sk_SK/omweather.mo
INSTALLS += transinstallsk
