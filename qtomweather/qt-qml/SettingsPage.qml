import Qt 4.7
import com.nokia.meego 1.0
//import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Page {
    id: settings
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }/*
        ToolIcon {
            platformStyle: ToolItemStyle {inverted: true}
            iconId: "toolbar-view-menu"
            onClicked: menu.open();
            anchors.right: parent == undefined ? undefined : parent.right
        }*/
    }
    orientationLock: PageOrientation.LockPortrait


}
