VERSION = 3.0
TEMPLATE = subdirs
CONFIG += ordered

#SUBDIRS += core qt-qml meego-mpl
SUBDIRS += core qtsetting qt-qml

CONFIG(meegopanel):SUBDIRS += netbook-UX
#system(pkg-config --exists meego-panel) {
# SUBDIRS += meego-mpl
#} 


