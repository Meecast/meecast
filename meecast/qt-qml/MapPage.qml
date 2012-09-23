//import QtQuick 1.1
import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: mapweather
    property int margin: 16
    property string map_pattern: ""
    property string filename: ""
    property string filenamebackground: ""
    property int totalImage: 4
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
    Rectangle {
        id: maprectangle
        property int velocity: 50
        property int currentImage: 0

        Image {
            id: mapimage
            source: filename 
//            anchors.fill: maprectangle
            opacity: 1
            // For Full Screen
            transform: Rotation { origin.x: 240; origin.y: 240; angle: 90}
         }
        Image {
            id: mapimagebackground
            source: filenamebackground 
//            anchors.fill: maprectangle
            opacity: 0
            // For Full Screen
            transform: Rotation { origin.x: 240; origin.y: 240; angle: 90}
        }

        ParallelAnimation {
           id: playgame
           running: true
           NumberAnimation { target: mapimage; property: "opacity"; to: 0; duration: 2000; }
           NumberAnimation { target: mapimagebackground; property: "opacity"; to: 1; duration: 2000; }
        }
        Timer {
            interval: 2000
            running: true;
            repeat: true;
            onTriggered: {mapimagebackground.opacity = 0; mapimage.opacity = 1; 
                          parent.currentImage = parent.currentImage + 1;
                          if (parent.currentImage >= totalImage) {
                                parent.currentImage = 0;
                          }
                          filename = map_pattern.replace("%s", parent.currentImage);
                          filenamebackground = map_pattern.replace("%s", (parent.currentImage + 1)); 
                          console.log (filename)
                          console.log (filenamebackground)

                          playgame.start()}
        }
    }
    Component.onCompleted: {
        console.log ("Preparing Map")
        console.log (map_pattern)
        filename = map_pattern.replace("%s", "0");
        filenamebackground = map_pattern.replace("%s", "1");
        console.log (filename)
        console.log (filenamebackground)
    }

}

