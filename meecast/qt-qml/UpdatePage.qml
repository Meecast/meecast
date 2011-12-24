import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: update 
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

            Label {
                text: Config.tr("Update interval")
                height: 80
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }
            ButtonColumn {
                width: parent.width
                platformStyle: ButtonStyle {
                    horizontalAlignment: Text.AlignLeft
                }
                Button {
                    text: Config.tr("Never")
                    checked: (!(Config.updateinterval == 1800 || Config.updateinterval == 3600 || Config.updateinterval == 7200 || Config.updateinterval == 14400))
                    onClicked: {
                        Config.updateinterval(2147483647);
                    }
	        }
                Button {
                    text: Config.tr("30 minutes")
                    checked: (Config.updateinterval == 1800)
                    onClicked: {
                        Config.updateinterval(1800);
                    }
	        }
                Button {
                    text: Config.tr("1 hour")
                    checked: (Config.updateinterval == 3600)
                    onClicked: {
                        Config.updateinterval(3600);
                    }
	        }
                Button {
                    text: Config.tr("2 hours")
                    checked: (Config.updateinterval == '7200')
                    onClicked: {
                        Config.updateinterval(7200);
                    }
	        }
                Button {
                    text: Config.tr("4 hours")
                    checked: (Config.updateinterval == '14400')
                    onClicked: {
                        Config.updateinterval(14400);
                    }
	        }

	    }
        }
    }
}
    

