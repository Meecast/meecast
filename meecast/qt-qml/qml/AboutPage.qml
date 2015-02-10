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
        contentHeight: 3300 
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
            text: Config.tr("MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright (C) 2006-2014")
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: abouttext.bottom
	    wrapMode: Text.Wrap                                                                                                                                            
            width: parent.width - 20
	}
    Label {
            id: projectsitetext 
            text: Config.tr("Support") + ":"
	    anchors.top: aboutfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
        signal clicked
        id: projectfulltext 
	    text: "support@meecast.org"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: projectsitetext.bottom
        MouseArea {
            id: mouseArea_web_support
            anchors.fill: parent
            onReleased: {
                Config.showwebsupport();
            }
        }

	}
//	Image {
//        signal clicked
//        id: donate_button
//	    source: Config.imagespath + "/btn_donate_LG.png" 
//	    anchors.top: projectsitetext.bottom
//	    anchors.rightMargin: 20
//	    anchors.right: parent.right
//        smooth: true	
//        MouseArea {
//            id: mouseArea
//            anchors.fill: parent
//            onReleased: {
//                Config.showwebdonation();
//            }
//        }
//
//    }

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
            text: Config.tr("Project administrators") + ":"
	    anchors.top: splitter.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: prohectadministatorfulltext 
	    text: "Vlad Vasilyeu, Oksana Kalinkevich"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: projectadministratortext.bottom
	}
    Label {
        id: leadprogrammertext 
        text: Config.tr ("Programmer") + ":"
	    anchors.top: prohectadministatorfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
    }
	Label {
        id: leadprogrammerfulltext 
	    text: "Vlad Vasilyeu, Tanya Makava"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: leadprogrammertext.bottom
	}
    Label {
        id: localizationtext
        text: Config.tr ("Localization platform") + ":"
	    anchors.top: leadprogrammerfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
    }
	Label {
        id: localizationfulltext 
	    text: "http://transifex.net"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: localizationtext.bottom
	}
	Label {
        id: leaddesignertext 
        text: Config.tr("Lead designers") + ":"
	    anchors.top: localizationfulltext.bottom 
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
        id: leaddesignerfulltext 
	    text: "Andrew Zhilin, Stephan Beyerle"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: leaddesignertext.bottom
	}
	Label {
            id: projectmanagertext 
            text: Config.tr("Project manager") + ":"
	    anchors.top: leaddesignerfulltext.bottom
	    anchors.topMargin: 30 
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    color: "#999999"
        }
	Label {
            id: projectmanagerfulltext 
	    text: "Ludmila Lisovskaya"
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: projectmanagertext.bottom
	}
        Label {
            id: translatorstext
            text: Config.tr("Translators") + ":"
            anchors.top: projectmanagerfulltext.bottom
            anchors.topMargin: 30
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "#999999"
        }
	Label {
            id: translatorsfulltext 
            text: Config.tr("\
French -  Nicolas Granziano, \n \
\tPhilippe, Benot Knecht, \n \
\tQuentin Caillard \n \
Russian - Pavel Fialko, Vlad Vasiliev, \n \
\tEd Bartosh\n \
Finnish - Marko Vertainen\n \
German -  Claudius Henrichs,\n \
\tJens Klingen,\n \
\tDeadHorseRiding\n \
Italian - Lorenzo Facca,\n \
\tPavel Fialko, Alessandro Pasotti,\n \
\tSalvatore Fiorenzo, Ilir Gjika,\n \
\tLuca Onnis\n \
Spanish - Alejandro Lopez, Agustí Clara\n \
Polish - Tomasz Dominikowski\n \
Slovak - Roman Moravčík\n \
Dutch - Tom Leenders, Michiel Pater\n \
Bulgarian - Zlatko Enikov\n \
Albanian - Ilir Gjika\n \
Chinese - Wong Man Kong, Michael\n \
Portuguese - Jose Narciso,\n \
Turkish - Burak Kaan Kose,\n \
\tMehmet Çilingir,\n \
Catalan - Agustí Clara,\n \
Arabic - Tarad Alfandi,\n \
Danish - Peter Jespersen,\n \
Serbian - Aleksandar Zaric,\n \
Norwegian - Peer-Atle Motland,\n \
Hungarian - Salmi,\n \
Slovenian -  Boštjan,\n \
Czech - Jakub Kožíšek, Pavel Šrubař,\n \
Chinese(Taiwan) - osk2, \n \
Vietnamese - Hieu Nguyen\n \
\tAnh Dương and truongnx, \n \
Swedish - Angela Nennebrandt,\n \
\tNiklas Åkerström, \n \
\tTomislav Veleckovik")
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: translatorstext.bottom
	}
    Label {
            id: iconsetext  
            text: Config.tr("Iconsets") + ":"
            anchors.top: translatorsfulltext.bottom
            anchors.topMargin: 30
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "#999999"
        }
	Label {
            id: iconsetfulltext 
	        text: "Beginning - \n \
Beginning beginning_somehow(at)yahoo.com \n\n\
Contour - \n\
Glance  - \n\
MeeCast - \n\
Andrew Zhilin  http://tabletui.wordpress.com \n\n\
Epona   - \n\
Gianni Polito aka Epona http://crystalxp.net \n\n\
Grzankas  -\n\
Wojciech Grzanka http://http://grzanka.pl/ \n\n\
SimpelMee - \n\
Kim Venetvirta  tardolus(at)ovi.com \n\n\
Noun   - \n\
designed by Adam Whitcroft \n\
from The Noun Project. http://thenounproject.com\n\n\
Shiny - \n\
JyriK http://jyrik.deviantart.com \n\
jyrik(at)koti.luukku.com";
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: iconsetext.bottom
	    font.pixelSize: 20 
	}
    Label {
            id: licensetext  
            text: Config.tr("License") + ":"
            anchors.top: iconsetfulltext.bottom
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
	Label {
            id: sourcetext  
            text: Config.tr("Source code") + ":"
            anchors.top: licensefulltext.bottom
            anchors.topMargin: 30
            anchors.leftMargin: 20
            anchors.left: parent.left
            color: "#999999"
    }
	Label {
        id: sourcefulltext 
	    text: Config.tr ("http://github.com/Meecast/meecast");
	    anchors.leftMargin: 20
	    anchors.left: parent.left
	    anchors.top: sourcetext.bottom
	}
                                                                                                    
    }
}
