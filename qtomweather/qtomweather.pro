VERSION = 3.0
TEMPLATE = subdirs
CONFIG += ordered

#SUBDIRS += core qt-qml meego-mpl
SUBDIRS += core qt-qml qtsetting xml-qml

CONFIG(meegopanel):SUBDIRS += netbook-UX
CONFIG(UXpanel):SUBDIRS += meego-UX-panel 
#system(pkg-config --exists meego-panel) {
# SUBDIRS += meego-mpl
#} 


