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
//        clip: true
        contentHeight: 1450
        Label {
            id: titletext
            text: Config.tr("MeeCast for Harmattan")
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
            text: Config.tr("About") + ":"
	    anchors.top: versiontext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: aboutfulltext 
            text: Config.tr("MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright (C) 2006-2011")
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: abouttext.bottom
	    wrapMode: Text.Wrap                                                                                                                                            
            width: parent.width - 20
	}
        Label {
            id: projectsitetext 
            text: Config.tr("Project website") + ":"
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
            text: Config.tr("Project administrator") + ":"
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
            text: Config.tr ("Lead programmers") + ":"
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
            text: Config.tr("Lead designer") + ":"
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
        Label {
            id: translatorstext
            text: Config.tr("Translators") + ":"
            anchors.top: leaddesignerfulltext.bottom
            anchors.topMargin: 30
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "#999999"
        }
	Label {
            id: translatorsfulltext 
	    text: Config.tr ("\
French -  Nicolas Granziano, \n \
\tPhilippe, Benot Knecht \n \
Russian - Pavel Fialko, Vlad Vasiliev, \n \
\tEd Bartosh\n \
Finnish - Marko Vertainen\n \
German -  Claudius Henrichs\n \
Italian - Pavel Fialko, Alessandro Pasotti,\n \
\tSalvatore Fiorenzo, Luca Onnis\n \
Spanish - Alejandro Lopez\n \
Polish - Tomasz Dominikowski\n \
Slovak - Roman Moravčík \n \
Swedish - Angela Nennebrandt")
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: translatorstext.bottom
	}
        Label {
            id: licensetext  
            text: Config.tr("License") + ":"
            anchors.top: translatorsfulltext.bottom
            anchors.topMargin: 30
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "#999999"
        }
	Label {
            id: licensefulltext 
	    text: Config.tr ("This software is free software; you can\n \
redistribute it and/or modify it under \n \
the terms of the GNU General Public\n \
License as published by the Free Software \n \
Foundation; either version 2.1 of the \n \
License, or (at your option) any \n \
later version.");
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: licensetext.bottom
	}

	                                                                                                    
    }
}
