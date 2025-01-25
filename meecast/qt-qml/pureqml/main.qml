import QtQuick 2.2
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import "pages"

ApplicationWindow {
    StackView {
        FontLoader {
            id: faSolid
            source: "qrc:///fonts/fa-solid-900.ttf"
        }
        id: pageStack
        visible: true
        anchors.fill: parent
        initialItem: WeatherWrapper {}
        Component.onDestruction : {   
            console.log("Component.onDestruction")
        }
    }
}
