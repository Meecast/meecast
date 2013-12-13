import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: countrypage
    property int margin: 16
    property string source: ""
    property int source_id: -1
    ListModel {
        id: listModel
        function update(my_text) {
            clear()
            for (var i=0; i<country_model.rowCount(); i++) {
                if (my_text == "" || country_model.get(i).name.indexOf(my_text) >= 0) {
                    append({"name": country_model.get(i).name, "key": country_model.get(i).key})
                }
            }
        }
        Component.onCompleted: update("")
    }
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
        PageHeader {
            title: Config.tr("Select country")
        }

        SilicaListView {
            id: countrylist
            anchors.fill: parent
            anchors.topMargin: 100
            anchors.bottom: parent.bottom
            width: parent.width
            model: listModel

            header: SearchField {
                        id: searchField
                        width: parent.width
                        onTextChanged: {
                            listModel.update(text)
                            forceActiveFocus()
                        }
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
                        //console.log("key = "+model.key+" name="+model.name);
                        region_model.populate(source, model.key);
                        if (region_model.count == 1){ // if one region found push to city page
                            city_model.populate(source, region_model.get(0).key);
                            pageStack.push(Qt.resolvedUrl("CityPage.qml"),
                                           {source: source, source_id: source_id,
                                            country_name: model.name, region_name: region_model.get(0).name}
                            );
                        }else {
                            console.log(model.name)
                            pageStack.push(Qt.resolvedUrl("RegionPage.qml"),
                                       {source: source, source_id: source_id, country_name: model.name}
                                       );
                        }
                    }
                }
            }
        }
    }
}

