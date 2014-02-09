import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
//import "cover"

ApplicationWindow {
    id: app
    property string temperature : ""
    initialPage: WeatherWrapper {}
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
  //  cover: CoverPage {} 
    Component.onDestruction : {   
        console.log("Component.onDestruction")
    }
}

