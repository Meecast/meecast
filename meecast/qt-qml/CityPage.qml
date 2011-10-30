import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: citypage
    property int margin: 16
    property string source: ""
    property int source_id: -1
    property string country_name: ""
    property string region_name: ""
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

    }
    orientationLock: PageOrientation.LockPortrait
    Rectangle{
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin

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
        ListView {
            id: citylist
            //anchors.fill: parent
            anchors.top: search.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            model: city_model

            delegate: Item {
                height: 80
                width: parent.width

                Label {
                    anchors.left: parent.left
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
                        pageStack.pop(pageStack.find(function(page) { return page.objectName == "stationspage" }));
                    }
                }
            }
            /*
            section {
            property: "category"
            criteria: ViewSection.FullString
            delegate: Rectangle {
                color: "black"
                width: parent.width
                height: childrenRect.height + 4
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pointSize: 24
                    text: section
                    color: "white"
                }
            }

        }
        ScrollDecorator {
            flickableItem: citylist
        }*/
                /*AdaptiveSearch {
                    id: adaptive
                    anchors.fill: parent
                    model: parent.model

                    onFilterUpdated: {
                        citylist.model = adaptive.filtermodel
                    }
                }*/
            }
        /*
        SectionScroller {
        listView: citylist
    }*/
        SearchField {
            id: search
            model: citylist.model
            anchors.top: parent.top
            width: parent.width
            height: 80
            onFiltered: {
                citylist.model = search.filtermodel;
            }
        }
    }
    Rectangle {
        id: title_rect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 80
        color: "black"
        Label {
            id: title
            anchors.fill: parent
            color: "white"
            text: Config.tr("Select location")
            //font.family: "NokiaPureMgLight"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

}

