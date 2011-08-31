import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0
//import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Page {
    id: stations
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
        ToolIcon {
            iconId: "toolbar-add"
            onClicked: {stations.openFile("NewStationPage.qml")}
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

    Item {
        id: mainitem
        anchors.fill: parent

        Label {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            text: "Manage locations"
            font.pointSize: 24
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        ListView {
            id: stationslist
            anchors.fill: parent
            anchors.top: title.bottom
            anchors.topMargin: margin

            model: Config.stations()

            delegate: Item {
                width: parent.width
                height: 80
                Label {
                    text: modelData
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: margin
                }
                Button {
                    //text: "Delete"
                    iconSource: "image://theme/icon-m-toolbar-cancle-white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    width: 50
                    onClicked: {
                        Config.removeStation(index);
                        stationslist.model = Config.stations();
                    }
                }
            }
        }
    }
}
