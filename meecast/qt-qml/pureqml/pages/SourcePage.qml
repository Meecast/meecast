import QtQuick 2.2
//import Sailfish.Silica 1.0
import QtQuick.Controls 2.12

Page {
    id: sourcepage
    property int margin: Theme.paddingSmall
    Rectangle{
        anchors.fill: parent
        color: Config.transparency ? "transparent" : "black"
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 80
            width: parent.width
            height: 610
            color: "transparent"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 80
            width: parent.width
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            anchors.topMargin: 80
            width: parent.width
            height: parent.height - 610
            color: "transparent"
        }

        ListView {
            id: sourcelist
            anchors.fill: parent
            model: source_model
            //header: PageHeader {
            //    title: Config.tr("Select the weather source")
            //}
            delegate: Item {
                //height: Theme.itemSizeSmall
                height: 100
                width: parent.width

                Label {
                    anchors.left: parent.left 
                    anchors.leftMargin: margin
                    anchors.verticalCenter: parent.verticalCenter
                    text: model.name
                }
                Image {
                    source: "image://theme/icon-m-right"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        country_model.populate(model.name);
                        pageStack.push(Qt.resolvedUrl("CountryPage.qml"),
                                       {source: model.name, source_id: index}
                                       );
                    }
                }
            }
        }
    }
}

