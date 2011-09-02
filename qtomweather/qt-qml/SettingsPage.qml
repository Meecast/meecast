import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: settings
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }/*
        ToolIcon {
            platformStyle: ToolItemStyle {inverted: true}
            iconId: "toolbar-view-menu"
            onClicked: menu.open();
            anchors.right: parent == undefined ? undefined : parent.right
        }*/
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
    ListModel {
        id: settingsModel
        ListElement {
            page: "StationsPage.qml"
            title: "Manage locations"
        }
        ListElement {
            page: "UnitsPage.qml"
            title: "Units"
        }/*
        ListElement {
            page: "UpdatePage.qml"
            title: "Update"
        }*/
        ListElement {
            page: "VisualsPage.qml"
            title: "Visuals"
        }
    }

    Label {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        text: Config.tr("Settings")
        font.pixelSize: 28
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    ListView {
        id: listview
        model: settingsModel
        anchors.fill: parent
        anchors.top: title.bottom
        anchors.topMargin: 30
        anchors.leftMargin: margin
        anchors.rightMargin: margin

        delegate: Item {
            height: 80
            width: parent.width

            Label {
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                text: Config.tr(model.title)
            }

            Image {
                source: "image://theme/icon-m-common-drilldown-arrow-inverse"
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    settings.openFile(model.page);
                }
            }
        }
    }
    ScrollDecorator {
        flickableItem: listview
    }

}
