//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: fullweather
    property int margin: 16
    property string station: ""
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }
    }
    orientationLock: PageOrientation.LockPortrait
    Flickable {
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        clip: true
        Label {
            text: "day view " + station
        }
    }
}

