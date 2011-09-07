import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: countrypage
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

    }
    orientationLock: PageOrientation.LockPortrait


        Label {
            id: title
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            text: Config.tr("Countries")
            font.pixelSize: 28
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        ListView {
            id: countrylist
            //anchors.top: title.bottom
            anchors.fill: parent
            //width: parent.width
            //height: model.rowCount() * 50
            model: temp_model
            //interactive: true

            delegate: Text {
                text: model.name
                color: "white"
                font.pointSize: 30
                height: 50
            }
            section {
                property: "category"
                criteria: ViewSection.FullString
                delegate: Rectangle {
                    color: "lightblue"
                    width: parent.width
                    height: childrenRect.height + 4
                    Text {
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.pointSize: 24
                        text: section
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

