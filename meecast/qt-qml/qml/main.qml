import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"

ApplicationWindow {
    //initialPage: WeatherPage {}
    initialPage: WeatherWrapper {}
    cover: Qt.resolvedUrl("cover/CoverPage.qml")
}

