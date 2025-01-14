import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: visuals
    property int margin: Theme.paddingSmall
    Rectangle {
        anchors.fill: parent
        color: {
            if (Config.transparency == "transparent"){
                return "transparent"
            }else{
                if (Theme.primaryColor == "#000000"){
                    return  Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                }else{
                    return "black"
                }
            }
        }
     }


  //  PageHeader {
  //      title: Config.tr("Appearance")
  //  }

    function openFile(file)
    {
        pageStack.push(Qt.resolvedUrl(file))
    }

    Rectangle{
        anchors.fill: parent
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "transparent"

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            color: "transparent"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 610
            color: "transparent"
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height
        contentWidth: parent.width
        VerticalScrollDecorator {}
        Column {
            id: column
            spacing: Theme.paddingLarge
            width: parent.width
            PageHeader {
                title: Config.tr("Appearance")
            }
            Row {
                width: parent.width
                height: 80
                Label {
                    //anchors.leftMargin: margin
                    anchors.leftMargin: Theme.paddingLarge 
                    text: Config.tr("Iconset") + ": " + Config.iconset
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Image {
                    source: "image://theme/icon-m-right"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        visuals.openFile("IconsetPage.qml");
                    }
                }
            }
            Row {
                width: parent.width
               // height: 80
                TextSwitch {
                    id: transparency
                    text: Config.tr("Transparency")
                    checked: Config.transparency
                    onCheckedChanged: {
                        if (Config.transparency != checked)   
                            Config.settransparency(transparency.checked);
                    }
                }
            }
            /*
            Row {
                width: parent.width
                TextSwitch {
                    id: lockscreen 
                    text: Config.tr("Widget in LockScreen")
                    checked: Config.lockscreen
                    onCheckedChanged: {
                        if (Config.lockscreen != checked)   
                            if (!Config.setlockscreen(lockscreen.checked)){
                                pageStack.push("NeedDaemonDialog.qml", {})
                                checked = false
                            }    
                    }
                }
            }
            Row {
                width: parent.width
                anchors.leftMargin: Theme.paddingLarge 
                anchors.left: parent.left
                Rectangle {
            	    width: parent.width
            	    height: 130 
                    color: "transparent"
                    Label {
                        id: first_phrase 
                        width: parent.width - 50
                        wrapMode:  TextEdit.WordWrap
                        text: Config.tr("You can also install real QML Lockscreen applet from:")
                    }
                    Button {
                        id: button_phrase
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: first_phrase.bottom
                        text: "http://openrepos.net"
                        onClicked: Qt.openUrlExternally("https://openrepos.net/content/vasvlad/meecast-lockscreen") 
                    }
        	    }
            }
            */

            SectionHeader {
                text: "Cover Page"
            }
            Row {
                width: parent.width
               // height: 80
                TextSwitch {
                    id: logocover
                    text: Config.tr("Show source logo")
                    checked: Config.logocoverpage
                    onCheckedChanged: {
                        if (Config.logocoverpage != checked)
                            Config.setlogocoverpage(logocover.checked);
                    }
                }
            }
            Row {
                width: parent.width
                TextSwitch {
                    id: windcover
                    text: Config.tr("Show wind")
                    checked: Config.windcoverpage
                    onCheckedChanged: {
                        if (Config.windcoverpage != checked)
                            Config.setwindcoverpage(windcover.checked);
                    }
                }
            }
            Row {
                width: parent.width
                TextSwitch {
                    id: lastupdatecover
                    text: Config.tr("Show last update time")
                    checked: Config.lastupdatecoverpage
                    onCheckedChanged: {
                        if (Config.lastupdatecoverpage != checked)
                            Config.setlastupdatecoverpage(lastupdatecover.checked);
                    }
                }
            }
        }
    }
/*
    MySelectionDialog {
        id: icon_dlg
        model: Config.icon_list()
        titleText: Config.tr("Icons")
        selectedIndex: -1
        onAccepted: {
            Config.set_iconset(selectedText);
        }
    }

    Dialog {
	id: enableDialog
	title: Item {
	    id: titleField
	    height: enableDialog.platformStyle.titleBarHeight
	    width: parent.width

	    Label {
		id: titleLabel
		anchors.verticalCenter: titleField.verticalCenter
		anchors.horizontalCenter: titleField.horizontalCenter
		font.capitalization: Font.MixedCase
		color: "white"
		text: Config.tr("Information")
	    }

	    Image {
		id: closeButton
		anchors.verticalCenter: titleField.verticalCenter
		anchors.right: titleField.right
		source: "image://theme/icon-m-framework-close"
		MouseArea {
		    id: closeButtonArea
		    anchors.fill: parent
		    onClicked:  { enableDialog.close() }
		}
	    }
	}

	content:Item {
	    id: name
	    height: childrenRect.height
	    Rectangle {
		id: black_rect1
		anchors.top: text.bottom
		height: 40
		color: "black"
	    }
	    Text {
		id: text
	    	width: parent.width
		font.pixelSize: 23
		color: "white"
		text: Config.tr("Application manager will now start\nfor installing package 'MeeCast applet'.\nYou should uninstall AccuWeather \nbefore installing this package")
	    }
	    Rectangle {
		id: black_rect
		anchors.top: text.bottom
		height: 40
		color: "black"
	    }
	}

	buttons: ButtonRow {
	    platformStyle: ButtonStyle { }
	    anchors.horizontalCenter: parent.horizontalCenter
            Button {id: b2; 
                   text: Config.tr("Cancel"); 
                   onClicked: {
                       eventwidget.checked = false
                       enableDialog.close()
                   }
            }
	    Button {id: b1; 
                    text: Config.tr("OK"); 
                    onClicked: {
		        Config.seteventwidget(eventwidget.checked)
                        event_widget_status = eventwidget.checked;                  
                        enableDialog.close()
                    }
            }
	}
    }
    Dialog {
	id: disableDialog
	title: Item {
	    id: titleField2
	    height: disableDialog.platformStyle.titleBarHeight
	    width: parent.width

	    Label {
            id: titleLabel2
            anchors.verticalCenter: titleField2.verticalCenter
            font.capitalization: Font.MixedCase
            color: "white"
            text: Config.tr("Information")
	    }

	    Image {
		id: closeButton2
		anchors.verticalCenter: titleField2.verticalCenter
		anchors.right: titleField2.right
		source: "image://theme/icon-m-framework-close"
		MouseArea {
		    id: closeButtonArea2
		    anchors.fill: parent
		    onClicked:  { disableDialog.close() }
		}
	    }
	}
    Dialog {
	id: warningDialog
	title: Item {
	    id: titleField3
	    height: disableDialog.platformStyle.titleBarHeight
	    width: parent.width

	    Label {
		id: titleLabel3
		anchors.verticalCenter: titleField3.verticalCenter
		font.capitalization: Font.MixedCase
		color: "white"
		text: Config.tr("Information")
	    }

	    Image {
		id: closeButton3
		anchors.verticalCenter: titleField3.verticalCenter
		anchors.right: titleField3.right
		source: "image://theme/icon-m-framework-close"
		MouseArea {
		    id: closeButtonArea3
		    anchors.fill: parent
		    onClicked:  { disableDialog.close() }
		}
	    }
	}

	content:Item {
	    id: name2
	    height: childrenRect.height
	    Rectangle {
		id: black_rect12
		anchors.bottom: text2.top
		height: 40
		color: "black"
	    }

	    Text {
		id: text2
		font.pixelSize: 22
		color: "white"
		text: Config.tr("You should activate \n'Widget in events view'\n for using LockScreen widget")
	    }	
	    Rectangle {
		id: black_rect2
		anchors.top: text2.bottom
		height: 40
		color: "black"
	    }
	}

	buttons: ButtonRow {
	    platformStyle: ButtonStyle { }
	    anchors.horizontalCenter: parent.horizontalCenter
	    Button {id: b22; text: Config.tr("Ok"); 
                    onClicked: {
                        warningDialog.close()
                        lockscreen.checked = false 
                    }
            }
	}
    }
    Dialog {
	id: warningDialog4
	title: Item {
	    id: titleField4
	    height: disableDialog.platformStyle.titleBarHeight
	    width: parent.width

	    Label {
		id: titleLabel4
		anchors.verticalCenter: titleField3.verticalCenter
		font.capitalization: Font.MixedCase
		color: "white"
		text: Config.tr("Information")
	    }

	    Image {
		id: closeButton4
		anchors.verticalCenter: titleField3.verticalCenter
		anchors.right: titleField4.right
		source: "image://theme/icon-m-framework-close"
		MouseArea {
		    id: closeButtonArea4
		    anchors.fill: parent
		    onClicked:  { disableDialog.close() }
		}
	    }
	}

	content:Item {
	    id: name24
	    height: childrenRect.height
	    Rectangle {
		id: black_rect124
		anchors.bottom: text2.top
		height: 40
		color: "black"
	    }

	    Text {
		id: text24
		font.pixelSize: 22
		color: "white"
		text: Config.tr("You should activate \n'Widget in events view'\n for using StandbyScreen widget")
	    }	
	    Rectangle {
		id: black_rect24
		anchors.top: text24.bottom
		height: 40
		color: "black"
	    }
	}

	buttons: ButtonRow {
	    platformStyle: ButtonStyle { }
	    anchors.horizontalCenter: parent.horizontalCenter
	    Button {id: b24; text: Config.tr("Ok"); 
                    onClicked: {
                        warningDialog4.close()
                        standbyscreen.checked = false 
                    }
            }
	}
    }

	content:Item {
	    id: name3
	    height: childrenRect.height
	    Rectangle {
		id: black_rect13
		anchors.bottom: text2.top
		height: 40
		color: "black"
	    }

	    Text {
		id: text3
		font.pixelSize: 22
		color: "white"
		text: Config.tr("For disabling MeeCast widget,\nyou should uninstall package\n'Activating MeeCast Applet'\nusing Application manager")
	    }	
	    Rectangle {
		id: black_rect3
		anchors.top: text2.bottom
		height: 40
		color: "black"
	    }
	}

	buttons: ButtonRow {
	    platformStyle: ButtonStyle { }
	    anchors.horizontalCenter: parent.horizontalCenter
	    Button {id: b23; text: Config.tr("Ok"); 
                    onClicked: {
                        eventwidget.checked = true 
                        disableDialog.close()
                    }
            }
	}
    }
    */
   /*
    */
}

