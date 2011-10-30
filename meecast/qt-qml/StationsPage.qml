import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0
//import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Page {
    id: stations
    objectName: "stationspage"
    property int margin: 16
    property int removedStation: -1
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
        ToolIcon {
            iconId: "toolbar-add"
            onClicked: {stations.openFile("SourcePage.qml")}
            anchors.right: parent == undefined ? undefined : parent.right
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
    //Config {id: config1}
    Connections {
        target: Config
        onConfigChanged: {
            console.log("wwwwwwwww");
            stationslist.model = Config.stations();
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
        QueryDialog {
            id: confirm
            titleText: Config.tr("Confirmation")
            message: Config.tr("Just remove it?")
            acceptButtonText: Config.tr("Remove")
            rejectButtonText: Config.tr("Not remove");
            onAccepted: {
                if (removedStation > -1){
                    Config.removeStation(removedStation);
                    stationslist.model = Config.stations();
                }
            }
        }
        Column {
            anchors.fill: parent
        Item {
            width: parent.width
            height: 80
            Label {
                text: Config.tr("Find location via GPS")
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
            }
            Switch {
                //checked: !rootWindow.showStatusBar
                id: gps
                checked: Config.gps
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                onCheckedChanged: {
                    Config.setgps(gps.checked);
                }
                //platformStyle: SwitchStyle {inverted: true}
            }

        }
        ListView {
            id: stationslist
            //anchors.fill: parent
            width: parent.width
            height: parent.height - 80
            model: Config.stations()

            delegate: Item {
                width: parent.width
                height: 80
                Label {
                    text: modelData
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Button {
                    iconSource: "image://theme/icon-m-toolbar-cancle-white"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    width: 50
                    onClicked: {
                        removedStation = index;
                        confirm.open();
                    }
                }
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
            text: Config.tr("Manage locations")
            //font.family: "Nokia Pure Text Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
}
