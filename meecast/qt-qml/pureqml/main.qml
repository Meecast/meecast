import QtQuick 2.2
import QtQuick.Controls 2.12
import "pages"

StackView {
    id: pageStack
    visible: true
    anchors.fill: parent
    initialItem: WeatherWrapper {}
    Component.onDestruction : {   
        console.log("Component.onDestruction")
    }
}

