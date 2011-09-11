import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: citypage
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

    ListView {
        id: citylist
        anchors.fill: parent
        model: city_model

        delegate: Component {
            id: listdelegate
            Item {
                width: parent.width
                height: 50
                Text {
                    text: model.name
                    color: "white"
                    font.pointSize: 30
                    height: 50
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log(model.key);
                        Config.saveStation1(model.key, model.name, region_name, country_name,
                                            source, source_id);
			pageStack.pop(pageStack.find(function(page) { return page.objectName == "stationspage" }));
                    }
                }
            }
        }
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
        }
    }
    SectionScroller {
        listView: citylist
    }
    Rectangle {
      id: labelrect
      anchors.top: parent.top
      anchors.left: parent.left
      width: parent.width
      color: "black" 
      border.color: "black"
      border.width: 3 
      height: 32
	    Label {
		id: title
		anchors.top: parent.top
		anchors.left: parent.left
		width: parent.width
		text: Config.tr("Stations")
		font.pixelSize: 28
		horizontalAlignment: Text.AlignHCenter
		verticalAlignment: Text.AlignVCenter
	    }
    }


}

