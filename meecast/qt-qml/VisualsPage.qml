import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: visuals
    property int margin: 16
    property bool event_widget_status: Config.eventwidget
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
    function openFile(file)
    {
        var component = Qt.createComponent(file);
        if (component.status == Component.Ready){
            pageStack.push(component);
        }else {
            console.log("error open file "+file);
        }
    }

    function getIndex(model, value)
    {
        var i=0;
        while (i<model.length && model[i]!=value) i++;
        if (i == model.length) return -1;
        else return i;
    }

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
		text: Config.tr("For disabling MeeCast widget,\nyou should uninstall package\n'Activating MeeCast Applet'\nusing Application manager")
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
                        eventwidget.checked = true 
                        disableDialog.close()
                    }
            }
	}
    }

    Rectangle{
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            color: "#999999"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 274
            color: "black"
        }
    Column {
        anchors.fill: parent
        //spacing: 20
        /*
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: Config.tr("Iconset")+": "+Config.iconset
            onClicked: {
                icon_dlg.open();
            }
            Component.onCompleted: {
                icon_dlg.selectedIndex = units.getIndex(icon_dlg.model, Config.iconset);
            }
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: Config.tr("FullScreen")
            checkable: true
            checked: !rootWindow.showStatusBar
            onClicked: {
                rootWindow.showStatusBar = !rootWindow.showStatusBar;
            }
        }*/
        Item {
            width: parent.width
            height: 80
            Label {
                text: Config.tr("Fullscreen mode")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Switch {
                //checked: !rootWindow.showStatusBar
                id: fullscreen
                checked: Config.fullscreen
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
                    rootWindow.showStatusBar = !fullscreen.checked;
                    Config.setfullscreen(fullscreen.checked);
                }
                //platformStyle: SwitchStyle {inverted: true}
            }

        }
        Item {
            width: parent.width
            height: 80
            Label {
                text: Config.tr("Iconset")+": "+Config.iconset
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Image {
                source: "image://theme/icon-m-common-drilldown-arrow-inverse"
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
        Item {
            width: parent.width
            height: 80
            Label {
                text: Config.tr("Widget in events view")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Switch {
                id: eventwidget 
                checked: Config.eventwidget
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
         	    if (event_widget_status != checked){          
                       if (checked)
                           enableDialog.open();
                       else
                           disableDialog.open();
		    }
                }
                //platformStyle: SwitchStyle {inverted: true}
            }

        }
        Item {
            width: parent.width
            height: 80
            Label {
                text: Config.tr("Widget in lockscreen")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Switch {
                id: lockscreen 
                checked: Config.lockscreen
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
         	    if (event_widget_status != checked){          
                       if (checked){
                           enableDialog.open();
                           Config.setlockscreen(lockscreen.checked);
		       }
                       else
                           Config.setlockscreen(lockscreen.checked);
		    }
                }
                //platformStyle: SwitchStyle {inverted: true}
            }

        }

    }
    }
    Rectangle {
        id: title_rect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 80
        color: "black"
        Label {
            id: title
            anchors.fill: parent
            color: "white"
            text: Config.tr("Appearance")
            //font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

}

