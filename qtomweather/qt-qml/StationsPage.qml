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

    Item {
        id: mainitem
        anchors.fill: parent

        ListView {
            id: stationslist
            anchors.fill: parent
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
                    text: "Delete"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: margin
                    width: 100
                    onClicked: {
                        Config.removeStation(index);
                        stationslist.model = Config.stations();
                    }
                }
            }
        }
    }
}
