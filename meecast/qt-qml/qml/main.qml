import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import "cover"

ApplicationWindow {
    id: app
    property string temperature : ""
    cover: CoverPage {} 
    initialPage: WeatherWrapper {}
}

