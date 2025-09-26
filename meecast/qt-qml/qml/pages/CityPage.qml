import QtQuick 2.2
import Sailfish.Silica 1.0
import "jsUtils.js" as Utils

Page {
    id: citypage
    allowedOrientations: Utils.decideOrientation();
    property int margin: Theme.paddingSmall
    property string source: ""
    property int source_id: -1
    property string country_name: ""
    property string region_name: ""
    ListModel {
        id: listModel
        function update(my_text) {
            clear()
            for (var i=0; i<city_model.rowCount(); i++) {
                if (my_text == "" || city_model.get(i).name.indexOf(my_text) >= 0) {
                    append({"name": city_model.get(i).name, "key": city_model.get(i).key})
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
            title: Config.tr("Select location")
        }
        SilicaListView {
            id: citylist
            anchors.fill: parent
            anchors.topMargin: 100
            anchors.bottom: parent.bottom
            width: parent.width
            currentIndex: -1
            model: listModel
            header:  SearchField {
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
                    source: "image://theme/icon-m-add"
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

