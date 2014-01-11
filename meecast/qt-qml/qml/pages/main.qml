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

    initialPage: Component { WeatherPage {} }
}
