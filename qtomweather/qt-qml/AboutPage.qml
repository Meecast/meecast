//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: about
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
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
            id: titletext
            text: "Meecast for Harmattan"
            anchors.horizontalCenter: parent.horizontalCenter
	    font.pixelSize: 32
        }
	Image {
            id: icon
	    source: Config.iconspath + "/" + Config.iconset + "/28.png" 
            anchors.horizontalCenter: parent.horizontalCenter
	    anchors.top: titletext.bottom
	    anchors.topMargin: 25
        }

    }
}
