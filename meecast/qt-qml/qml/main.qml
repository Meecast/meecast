//import Qt 4.7
import QtQuick 2.0
//import "." 1.0
//import com.nokia.meego 1.0
//import Qt.labs.components.native 1.0

import Sailfish.Silica 1.0

ApplicationWindow {
    id: rootWindow

//    platformStyle: defaultStyle
//    showStatusBar: !Config.fullscreen
    property bool menuitemgps: Config.gps

//    PageStackWindowStyle { id: defaultStyle }
//    PageStackWindowStyle {
//        id: customStyle
//        background: "image://theme/meegotouch-wallpaper-portrait";
//        backgroundFillMode: Image.PreserveAspectCrop
//    }
//    Component.onCompleted: {
//        theme.inverted = true;
//    }

    initialPage: WeatherPage {}

    function openFile(file)
    {
        var component = Qt.createComponent(file);
        if (component.status == Component.Ready){
            pageStack.push(component);
        }else {
            console.log("error open file "+file);
        }
    }

//    Menu {
//        id: myMenu
//        // visualParent: pageStack
//        MenuLayout {
//            MenuItem {
//                id: item1
//                text: Config.tr("Settings")
//                onClicked: {
//                    rootWindow.openFile("SettingsPage.qml");
//                }
//            }
//            MenuItem {
//                id: item2
//                visible: menuitemgps
//                text: Config.tr("Adjust GPS station")
//                onClicked: {
//                    Config.updategpsstation();
//                }
//            }
//            MenuItem {
//                id: item3
//                text: Config.tr("About")
//                onClicked: {
//                    rootWindow.openFile("AboutPage.qml");
//                }
//            }
//        }
//    }
//
}
