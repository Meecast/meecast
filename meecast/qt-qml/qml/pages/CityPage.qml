import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: citypage
    property int margin: 16
    property string source: ""
    property int source_id: -1
    property string country_name: ""
    property string region_name: ""
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
            id: citylist
            anchors.fill: parent
            anchors.bottom: parent.bottom
            width: parent.width
            model: city_model
            header: PageHeader {
                title: Config.tr("Select location")
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
                    source: "image://theme/icon-s-common-add-inverse"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        //console.log(model.key);
                        Config.saveStation1(model.key, model.name, region_name, country_name,
                                            source, source_id);
                        if (pageStack.find(function(page) { return page.objectName == "stationspage" })){
                            pageStack.pop(pageStack.find(function(page) { return page.objectName == "stationspage" }));
                        }else{
                            pageStack.pop(pageStack.find(function(page) { return page.objectName == "WeatherPage" }));
                        }
                    }
                }
            }
       }
    }
}

