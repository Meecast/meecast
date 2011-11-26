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
	id: myDialog
	title: Item {
	    id: titleField
	    height: myDialog.platformStyle.titleBarHeight
	    width: parent.width

	    Label {
		id: titleLabel
		anchors.left: supplement.right
		anchors.verticalCenter: titleField.verticalCenter
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
		    onClicked:  { myDialog.close() }
		}
	    }
	}

	content:Item {
	    id: name
	    height: childrenRect.height
	    Text {
		id: text
		font.pixelSize: 22
		color: "white"
		text: Config.tr("Application manager will now start \n"
                       +"for installing package 'MeeCast applet'.\n"
                       +"You should uninstall AccuWeather before \ninstalling this package")
	    }
	}

	buttons: ButtonRow {
	    platformStyle: ButtonStyle { }
	    anchors.horizontalCenter: parent.horizontalCenter
	    Button {id: b1; text: Config.tr("OK"); onClicked: Config.seteventwidget(eventwidget.checked)}
	    Button {id: b2; text: Config.tr("Cancel"); onClicked: myDialog.close()}
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
                //checked: false 
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
         	    if (event_widget_status != checked){          
                       if (checked)
                           myDialog.open();
                       else
                           myDialog.open();
		    }
                    Config.seteventwidget(eventwidget.checked);
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

