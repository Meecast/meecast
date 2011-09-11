import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: regionpage
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

    ListView {
        id: regionlist
        anchors.fill: parent
        model: region_model

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
                        city_model.populate(source, model.key);
                        pageStack.push(Qt.resolvedUrl("CityPage.qml"),
                                       {source: source, source_id: source_id,
                                       country_name: country_name, region_name: model.name}
                                       );

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
            flickableItem: regionlist
        }
    }
    SectionScroller {
        listView: regionlist
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
	text: Config.tr("Regions")
	font.pixelSize: 28
	horizontalAlignment: Text.AlignHCenter
	verticalAlignment: Text.AlignVCenter
      }
   }

}

