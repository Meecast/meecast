import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: regionpage
    property int margin: 16
    property string source: ""
    property int source_id: -1
    property string country_name: ""
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
            id: regionlist
            anchors.fill: parent
            anchors.top: title_rect.bottom
            model: region_model

            delegate: Item {
                height: 80
                width: parent.width

                Label {
                    anchors.left: parent.left
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
            flickableItem: regionlist
        }*/
                AdaptiveSearch {
                    id: adaptive
                    anchors.fill: parent
                    model: parent.model

                    onFilterUpdated: {
                        regionlist.model = adaptive.filtermodel
                    }
                }
            }
        /*
        SectionScroller {
        listView: regionlist
    }*/
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
            text: Config.tr("Select region")
            font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

}

