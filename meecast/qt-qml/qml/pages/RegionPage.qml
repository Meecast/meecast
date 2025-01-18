import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: regionpage
    property int margin: Theme.paddingSmall
    property string source: ""
    property int source_id: -1
    property string country_name: ""
    ListModel {
        id: listModel
        function update(my_text) {
            clear()
            for (var i=0; i<region_model.rowCount(); i++) {
                if (my_text == "" || region_model.get(i).name.indexOf(my_text) >= 0) {
                    append({"name": region_model.get(i).name, "key": region_model.get(i).key})
                }
            }
        }
        Component.onCompleted: update("")
    }

    Rectangle{
        anchors.fill: parent
        color: {
            if (Config.transparency){
                return "transparent"
            }else{
                if (Theme.primaryColor == "#000000"){
                    return  Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                }else{
                    return "black"
                }
            }
        }

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            anchors.topMargin: 80
            height: 610
            color: "transparent"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            anchors.topMargin: 80
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            anchors.topMargin: 80
            height: parent.height - 610
            color: "transparent"
        }
        PageHeader {
            title: Config.tr("Select region")
        }
        SilicaListView {
            id: regionlist
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
                height: Theme.itemSizeSmall
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

