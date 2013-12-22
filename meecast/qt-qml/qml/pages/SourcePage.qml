import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: sourcepage
    property int margin: 16
    Rectangle{
        anchors.fill: parent
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
            anchors.left: parent.left
            anchors.topMargin: 80
            width: parent.width
            height: 274
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            anchors.topMargin: 80
            width: parent.width
            height: parent.height - 274
            color: "black"
        }

        SilicaListView {
            id: sourcelist
            anchors.fill: parent
            model: source_model
            header: PageHeader {
                title: Config.tr("Select the weather source")
            }
            delegate: Item {
                height: 80
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

