import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: countrypage
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

    /*
    Label {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        text: Config.tr("Countries")
        font.pixelSize: 28
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }*/

    ListView {
        id: countrylist
        anchors.fill: parent
        model: country_model

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
                        region_model.populate(source, model.key);
                        pageStack.push(Qt.resolvedUrl("RegionPage.qml"),
                                       {source: source, source_id: source_id, country_name: model.name}
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
            flickableItem: countrylist
        }
    }
    SectionScroller {
        listView: countrylist
    }

}

