// import QtQuick 1.0
// import "content"

import Qt 4.7
import com.nokia.meego 1.0
Page {
    id: languagespage
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

    }


 
    Rectangle {
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



     // Define a delegate component.  A component will be
     // instantiated for each visible item in the list.
     Component {
         id: langDelegate
         Item {
             id: wrapper
             width: parent.width 
             height: 80
                Label {
                    id: lang_label
                    anchors.leftMargin: margin
                    anchors.left: parent.left 
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData
                }
                Image {
                    source: "image://theme/icon-m-common-drilldown-arrow-inverse"
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Config.set_language(modelData);
                        pageStack.pop();
                    }
                }

 
             // indent the item if it is the current item
             states: State {
                 name: "Current"
                 when: wrapper.ListView.isCurrentItem
             }
             transitions: Transition {
                 NumberAnimation { properties: "x"; duration: 200 }
             }
         }
     }

     // Define a highlight with customised movement between items.
     Component {
         id: highlightBar
         Rectangle {
             width: parent.width; height: 80
             color: "blue" 
             y: listView.currentItem.y;
             radius: 5
             Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
         }
     }

     ListView {
         id: listView
         width: parent.width; height: parent.height
         currentIndex : Config.index_of_current_language() 
         model: Config.languages_list() 
         delegate: langDelegate
         focus: true
         highlight: highlightBar
         highlightFollowsCurrentItem: true
         Component.onCompleted: { positionViewAtIndex(Config.index_of_current_language(), ListView.End);}
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
            text: Config.tr("Select the language")
            //font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
}
