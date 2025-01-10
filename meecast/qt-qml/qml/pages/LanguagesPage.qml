import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: languagespage
    property int screen_height : languagespage.height
    property int screen_width : languagespage.width 
    property int margin: Theme.paddingSmall
    Rectangle {
        anchors.fill: parent
        color: {
            if (config.transparency == "transparent"){
                return "transparent"
            }else{
                if (theme.primarycolor == "#000000"){
                    return  theme.rgba(theme.highlightbackgroundcolor, theme.highlightbackgroundopacity)
                }else{
                    return "black"
                }
            }
        }
    }

    PageHeader {
        title: Config.tr("Select the language")
    }

    Rectangle {
        anchors.fill: parent
        anchors.top: parent.top
        anchors.topMargin: 0.0805*screen_height
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "transparent" 
        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            color: "transparent"
        }

        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 610
            color: "transparent"
        }

        // Define a delegate component.  A component will be
        // instantiated for each visible item in the list.
        Component {
            id: langDelegate
            Item {
                id: wrapper
                width: parent.width 
                //height: 0.0805*screen_height
                height: Theme.itemSizeSmall
                   Label {
                       id: lang_label
                       anchors.leftMargin: margin
                       anchors.left: parent.left 
                       anchors.verticalCenter: parent.verticalCenter
                       text: modelData
                   }
                   Image {
                       source: "image://theme/icon-m-right"
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
                color: "#30ffffff" 
                y: listView.currentItem.y;
                radius: 5
                Behavior on y { SpringAnimation { spring: 2; damping: 0.1 } }
            }
        }
        
        SilicaListView {
            id: listView
            width: parent.width; 
            height: parent.height
            anchors.top: parent.top
           // anchors.topMargin: 80
            currentIndex : Config.index_of_current_language() 
            model: Config.languages_list() 
            delegate: langDelegate
            focus: true
            highlight: highlightBar
            highlightFollowsCurrentItem: true
            Component.onCompleted: { positionViewAtIndex(Config.index_of_current_language(), ListView.End);}
        }
    }
}
