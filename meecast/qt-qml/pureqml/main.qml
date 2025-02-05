import QtQuick 2.2
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import "pages"

ApplicationWindow {
    id: main_window
    StackView {
        Keys.onPressed: {
            event.accepted = true;
            if (event.key === Qt.Key_Left || event.key === Qt.Key_Back) {
                console.log("press back");
                if (pageStack.depth > 1) {
                    pageStack.pop();
                } else { Qt.quit(); }
            }
        }
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
