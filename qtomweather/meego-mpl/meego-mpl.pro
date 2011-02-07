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

for(language, TRANSLIST):TRANSLATIONS += ../po/$${language}.po
FORMS += mainwindow.ui \
    installer/installerform.ui
!isEmpty(TRANSLATIONS) {
    isEmpty(QMAKE_LRELEASE) {
        win32:QMAKE_LRELEASE = msgfmt.exe
        else:QMAKE_LRELEASE = msgfmt
    }
    TSQM.name = msgfmt \
        ${QMAKE_FILE_IN}
    TSQM.input = TRANSLATIONS
    TSQM.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.mo
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


transinstallbg.path = /usr/share/locale/bg/LC_MESSAGES
transinstallbg.files = ../po/bg_BG.mo
INSTALLS += transinstallbg

transinstallde.path = /usr/share/locale/de/LC_MESSAGES
transinstallde.files = ../po/de_DE.mo
INSTALLS += transinstallde

transinstallengb.path = /usr/share/locale/en_GB/LC_MESSAGES
transinstallengb.files = ../po/en_GB.mo
INSTALLS += transinstallengb

transinstallenus.path = /usr/share/locale/en_US/LC_MESSAGES
transinstallenus.files = ../po/en_US.mo
INSTALLS += transinstallenus

transinstalleses.path = /usr/share/locale/es_ES/LC_MESSAGES
transinstalleses.files = ../po/en_ES.mo
INSTALLS += transinstalleses

transinstallesmx.path = /usr/share/locale/es_MX/LC_MESSAGES
transinstallesmx.files = ../po/en_MX.mo
INSTALLS += transinstallesmx

transinstallfi.path = /usr/share/locale/fi/LC_MESSAGES
transinstallfi.files = ../po/fi_FI.mo
INSTALLS += transinstallfi

transinstallfr.path = /usr/share/locale/fr/LC_MESSAGES
transinstallfr.files = ../po/fr_FR.mo
INSTALLS += transinstallfr

transinstallit.path = /usr/share/locale/it/LC_MESSAGES
transinstallit.files = ../po/it_IT.mo
INSTALLS += transinstallit

transinstalllv.path = /usr/share/locale/lv/LC_MESSAGES
transinstalllv.files = ../po/lv_LV.mo
INSTALLS += transinstalllv

transinstallpl.path = /usr/share/locale/pl/LC_MESSAGES
transinstallpl.files = ../po/pl_PL.mo
INSTALLS += transinstallpl

transinstallru.path = /usr/share/locale/ru/LC_MESSAGES
transinstallru.files = ../po/ru_RU.mo
INSTALLS += transinstallru

transinstallsk.path = /usr/share/locale/sk/LC_MESSAGES
transinstallsk.files = ../po/sk_SK.mo
INSTALLS += transinstallsk
