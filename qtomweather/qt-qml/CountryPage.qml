import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: countrypage
    property int margin: 16
    property string source: ""
    property int source_id: -1
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
            id: countrylist
            //anchors.fill: parent
            anchors.top: search.bottom
            anchors.bottom: parent.bottom
            width: parent.width
            model: country_model

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
                        console.log("key = "+model.key+" name="+model.name);
                        region_model.populate(source, model.key);
                        pageStack.push(Qt.resolvedUrl("RegionPage.qml"),
                                       {source: source, source_id: source_id, country_name: model.name}
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
			width: countrypage.width
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
		    flickableItem: countrylist
                }*/
        /*
            AdaptiveSearch {
                id: adaptive
                anchors.fill: parent
                model: parent.model

                onFilterUpdated: {
                    countrylist.model = adaptive.filtermodel
                }
            }*/
        }/*
        SectionScroller {
		listView: countrylist
            }*/
        SearchField {
            id: search
            model: countrylist.model
            anchors.top: parent.top
            width: parent.width
            height: 80
            onFiltered: {
                countrylist.model = search.filtermodel;
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
            text: Config.tr("Select country")
            font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

}

