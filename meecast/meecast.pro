VERSION = 3.0
TEMPLATE = subdirs
CONFIG += ordered

#SUBDIRS += core qt-qml meego-mpl
linux:!android {
    message("* Using settings for Unix/Linux.")
    SUBDIRS += sqlite3 core qt-qml predeamon
}

android {
    SUBDIRS += sqlite3 libxml2 core qt-qml
}

#CONFIG(meegopanel):SUBDIRS += netbook-UX
#CONFIG(UXpanel):SUBDIRS += meego-UX-panel 
#system(pkg-config --exists meego-panel) {
# SUBDIRS += meego-mpl
#} 


