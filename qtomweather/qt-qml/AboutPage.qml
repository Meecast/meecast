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
            x: 0; y: 80; width: parent.width; height: 274
        }

	Image {
            id: backgroundmask 
	    source: Config.imagespath + "/mask_background.png"
            x: 0; y: 80; width: parent.width; 
	    smooth: true
        }
	Image {
            id: titlemask 
	    source: Config.imagespath + "/mask_title.png"
//            x: 0; y: 80; width: parent.width; 	
//	    anchors.bottom: versiontext.bottom
	    anchors.verticalCenter: versiontext.verticalCenter
            smooth: true	
        }


	Image {
            id: icon
	    source: Config.iconspath + "/" + Config.iconset + "/28.png" 
            anchors.horizontalCenter: parent.horizontalCenter
	    anchors.top: titletext.bottom
	    anchors.topMargin: 25
            smooth: true	
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
            text: qsTr("About") + ":"
	    anchors.top: versiontext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
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
        Label {
            id: projectsitetext 
            text: "Project website:"
	    anchors.top: aboutfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: projectfulltext 
	    text: "http://meecast.com"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: projectsitetext.bottom
	}
	Rectangle {
	    id: splitter
            color: "#999999"
            x: 20; width: parent.width - 40; height: 2
	    anchors.top: projectfulltext.bottom 
	    anchors.leftMargin: 20
	    anchors.topMargin: 30 
        }

        Label {
            id: projectadministratortext 
            text: "Project administrator:"
	    anchors.top: splitter.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: prohectadministatorfulltext 
	    text: "Vlad Vasilyeu"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: projectadministratortext.bottom
	}
        Label {
            id: leadprogrammertext 
            text: "Lead programmers:"
	    anchors.top: prohectadministatorfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: leadprogrammerfulltext 
	    text: "Tanya Makava, Vlad Vasilyeu"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: leadprogrammertext.bottom
	}
	Label {
            id: leaddesignertext 
            text: "Lead designer:"
	    anchors.top: leadprogrammerfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: leaddesignerfulltext 
	    text: "Andrew Zhilin"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: leaddesignertext.bottom
	}
    }
}
