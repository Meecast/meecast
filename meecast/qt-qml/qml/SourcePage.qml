import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: sourcepage
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

    }

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
            id: sourcelist
            anchors.fill: parent
            //anchors.top: search.bottom
            model: source_model

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
                        country_model.populate(model.name);
                        pageStack.push(Qt.resolvedUrl("CountryPage.qml"),
                                       {source: model.name, source_id: index}
                                       );
                    }
                }
            }
        }
        /*
        SearchField {
            id: search
            model: sourcelist.model
            anchors.top: parent.top
            width: parent.width
            height: 80
            onFiltered: {
                console.log(search.filter + " "+ search.filtermodel.count);
                sourcelist.model = search.filtermodel;
            }
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
            text: Config.tr("Select the weather source")
            //font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }


}

