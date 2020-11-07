TEMPLATE = lib
TARGET = meecastdata 
PLUGIN_IMPORT_PATH = org/meecast/data
QT += qml quick dbus
CONFIG += qt plugin dbus link_pkgconfig
PKGCONFIG += contentaction5

TARGET = $$qtLibraryTarget($$TARGET)
uri = org.meecast.data

# Input
SOURCES += \
    meecastdata_plugin.cpp \
    meecastdata.cpp

HEADERS += \
    meecastdata_plugin.h \
    meecastdata.h

OTHER_FILES = qmldir

patch.files += patch/patch.json 
patch.files += patch/unified_diff.patch 
patch.path = /usr/share/patchmanager/patches/sailfishos-lockscreen-meecast-patch 


!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
qmldir.path = $$[QT_INSTALL_QML]/$$$$PLUGIN_IMPORT_PATH
target.path = $$[QT_INSTALL_QML]/$$PLUGIN_IMPORT_PATH

exists("/usr/lib/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
    qmldir.path = /usr/lib/qt5/qml/$$$$PLUGIN_IMPORT_PATH
    target.path = /usr/lib/qt5/qml/$$PLUGIN_IMPORT_PATH
}

exists("/usr/lib64/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
    qmldir.path = /usr/lib64/qt5/qml/$$$$PLUGIN_IMPORT_PATH
    target.path = /usr/lib64/qt5/qml/$$PLUGIN_IMPORT_PATH
}




INSTALLS += target qmldir patch

