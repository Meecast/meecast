import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: stations
    objectName: "stationspage"
    property int margin: 16
    property int removedStation: -1
    property string removedStationName: ""

//    tools: ToolBarLayout {
//        ToolIcon {
//            iconId: "toolbar-back"
//            onClicked: {
//                pageStack.pop();
//            }
//        }
//        ToolIcon {
//            iconId: "toolbar-add"
//            onClicked: {stations.openFile("SourcePage.qml")}
//            anchors.right: parent == undefined ? undefined : parent.right
//        }
//    }
//    orientationLock: PageOrientation.LockPortrait
    function openFile(file)
    {
        pageStack.push(Qt.resolvedUrl(file))
    }
    Connections {
        target: Config
        onConfigChanged: {
            console.log("wwwwwwwww");
            stationslist.model = Config.stations();
        }
    }

    Rectangle{
       anchors.fill: parent
       color: "black"

        id: mainrect
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 80
            width: parent.width
            height: 274
            color: "#999999"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.topMargin: 80
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
        Component{
            id: confirm
            Dialog{
                Column{
                    DialogHeader {
                        title: removedStationName
                        acceptText: Config.tr("Yes")
                    }
                    Label {
                        text: Config.tr("Delete location?")
                    }
                }
                onAccepted: {
                    if (removedStation > -1){
                        Config.removeStation(removedStation);
                        stationslist.model = Config.stations();
                    }
                }
            }
        }
        Column {
            anchors.fill: parent
            id: maincolumn
            PageHeader {
                title: Config.tr("Manage locations")
            }
            Item {
                width: parent.width
 
                height: 80
                Label {
                    text: Config.tr("Find location via GPS")
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                    anchors.verticalCenter: parent.verticalCenter
                }
                Switch {
                    //checked: !rootWindow.showStatusBar
                    id: gps
                    checked: Config.gps
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    onCheckedChanged: {
                        console.log("swithc checked changed");
                        Config.setgps(gps.checked);
                        if (gps.checked){
                            rootWindow.menuitemgps = true
                        }else{
                            rootWindow.menuitemgps = false 
                        }
                    }
                }
            }
            SilicaListView {
                id: stationslist
                width: parent.width
                height: parent.height - 80
                model: Config.stations()

                delegate: Item {
                    width: parent.width
                    height: 80
 
                    Label {
                        text: modelData
                        anchors.left: parent.left
                        anchors.leftMargin: margin
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    IconButton {
                        icon.source: "image://theme/icon-l-delete"
                        anchors.right: parent.right
                        anchors.rightMargin: margin
                        anchors.verticalCenter: parent.verticalCenter
                        width: 50
                        onClicked: {
                            removedStation = index;
                            removedStationName = modelData;
                            var dialog = pageStack.push(confirm);
                        }
                    }
                }
            }
        }
    }
    Button {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        text: Config.tr("Add Station")
        onClicked: {stations.openFile("SourcePage.qml")}
    }
}
