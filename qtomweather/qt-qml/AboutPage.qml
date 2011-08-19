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
            text: "MeeCast for Harmattan"
            anchors.horizontalCenter: parent.horizontalCenter
	    font.pixelSize: 32
        }

        Rectangle {
            color: "purple"
            x: 0; y: 80; width: 480; height: 274
        }

	Image {
            id: backgroundmask 
	    source: Config.imagespath + "/mask_background.png"
            x: 0; y: 80; width: 480; height: 274
        }

	Image {
            id: icon
	    source: Config.iconspath + "/" + Config.iconset + "/28.png" 
            anchors.horizontalCenter: parent.horizontalCenter
	    anchors.top: titletext.bottom
	    anchors.topMargin: 25
        }

     	Label {
            id: versiontext
            text: "Version" + " " + Config.version 
            anchors.horizontalCenter: parent.horizontalCenter
	    anchors.top: icon.bottom
	    anchors.topMargin: 5
	    font.pixelSize: 24
        }
        Label {
            id: abouttext 
            text: "About:"
	    anchors.top: backgroundmask.bottom
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#303030"
        }
	Label {
            id: aboutfulltext 
	    text: "MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright @ 2006-2011"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: abouttext.bottom
	    wrapMode: Text.Wrap                                                                                                                                            
            width: parent.width - 20
	}


    }
}
