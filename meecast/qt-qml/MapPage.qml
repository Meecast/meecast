//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: mapweather
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }
   
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: {(myMenu.status == DialogStatus.Closed) ? myMenu.open() : myMenu.close()}
            anchors.right: parent == undefined ? undefined : parent.right
        }

    }
    orientationLock: PageOrientation.LockPortrait
    function openFile(file)
    {
        var component = Qt.createComponent(file);
        if (component.status == Component.Ready){
            pageStack.push(component);
        }else {
            console.log("error open file "+file);
        }
    }
    Image {
        id: mapimage
        source: "/home/vlad/.cache/com.meecast.omweather/weather.com_AFXX0005_map_0.png"
       // rotation: 90
        // For Full Screen
        transform: Rotation { origin.x: 240; origin.y: 240; angle: 90}
    }
}

