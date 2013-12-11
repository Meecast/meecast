import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: regionpage
    property int margin: 16
    property string source: ""
    property int source_id: -1
    property string country_name: ""
    Rectangle{
        anchors.fill: parent
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
        SilicaListView {
            id: regionlist
            anchors.fill: parent
            anchors.bottom: parent.bottom
            width: parent.width
            model: region_model
            header: PageHeader {
                title: Config.tr("Select region")
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
                    source: "image://theme/icon-m-common-drilldown-arrow-inverse"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        //console.log(model.key);
                        city_model.populate(source, model.key);
                        pageStack.push(Qt.resolvedUrl("CityPage.qml"),
                                       {source: source, source_id: source_id,
                                        country_name: country_name, region_name: model.name}
                        );
                    }
                }
            }
        }
    }
}

