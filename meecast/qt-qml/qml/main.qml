import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
//import "cover"

ApplicationWindow {
    id: app
    initialPage: WeatherWrapper {}
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
    Component.onDestruction : {   
        console.log("Component.onDestruction")
    }
}

