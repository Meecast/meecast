TEMPLATE = lib
TARGET = weather 
QT += qml quick dbus
CONFIG += qt plugin dbus

TARGET = $$qtLibraryTarget($$TARGET)
uri = Sailfish.Weather 

# Input
SOURCES += \
    weather_plugin.cpp \
    weather.cpp

HEADERS += \
    weather_plugin.h \
    weather.h

OTHER_FILES = qmldir

qml.files += WeatherBanner.qml
qml.files += WeatherIndicator.qml
contains(QT_ARCH, arm64) {
    qml.path = /usr/lib64/qt5/qml/Sailfish/Weather
} else {
    qml.path = /usr/lib/qt5/qml/Sailfish/Weather
}


!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    exists("/usr/lib/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
        qmldir.path = /usr/lib/qt5/qml/Sailfish/Weather
        target.path = /usr/lib/qt5/qml/Sailfish/Weather
    }

    exists("/usr/lib64/qt5/qml/Sailfish/Silica/SilicaGridView.qml"): {
        qmldir.path = /usr/lib64/qt5/qml/Sailfish/Weather
        target.path = /usr/lib64/qt5/qml/Sailfish/Weather
    }

    INSTALLS += target qmldir qml 
}

