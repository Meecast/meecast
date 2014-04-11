import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: iconsetpage
    property int margin: 16
    Rectangle {
        anchors.fill: parent
        color: Config.transparency ? "transparent" : "black"
    }

    PageHeader {
        title: Config.tr("Select the iconset")
    }

    Rectangle{
        anchors.fill: parent
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color:  "transparent" 

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            color: Config.transparency ? "transparent" : "#999999"
        }
        Loader {
            id: background
            visible: Config.transparency ? false : true
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
            color: Config.transparency ? "transparent" : "black"
        }

        SilicaListView {
            id: sourcelist
            anchors.fill: parent
            model: Config.icon_list()

            delegate: Item {
                height: 80
                width: parent.width

                Label {
                    anchors.left: iconset_icon.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData
                }

                Image {
                    id: iconset_icon
                    width: 64
                    height: 64
                    source: Config.iconspath + "/" + modelData + "/" + "28.png"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Config.set_iconset(modelData);
                        pageStack.pop();
                    }
                }
            }
        }
    }
}


