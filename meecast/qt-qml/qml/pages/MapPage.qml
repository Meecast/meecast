import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: mapweather
    property int margin: 16
    property string map_pattern: ""
    property string filename: ""
    property string mapbackground_pattern: ""
    property string filenamebackground: ""
    property string count_of_maps: ""
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
        property int currentImage: count_of_maps 
        property int round_robin_count: 0

        Image {
            id: basemapimage
            source: filenamebackground 
            opacity: 1
            // For Full Screen
            transform: Rotation { origin.x: 240; origin.y: 240; angle: 90}
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//		             if (!map_timer.running){
//                         map_timer.running = true;
//                         map_timer.repeat = true;
//			         }else{
//                        map_timer.running = false;
//                     }
//                     console.log("Map onclicked");
//                }
//            }

         }


        Image {
            id: mapimage
            source: filename 
            opacity: 1
            // For Full Screen
            transform: Rotation { origin.x: 240; origin.y: 240; angle: 90}
            MouseArea {
                anchors.fill: parent
                onClicked: {
		             if (!map_timer.running){
                         map_timer.running = true;
                         map_timer.repeat = true;
			         }else{
                        map_timer.running = false;
                     }
                     console.log("Map onclicked");
                }
            }

         }

        Timer {
            interval: 1000
            running: true;
            repeat: true;
            id: map_timer
            onTriggered: {
                     parent.currentImage = parent.currentImage - 1;
                     if (parent.currentImage < 1) 
                         parent.currentImage = count_of_maps;
                     filename = map_pattern.replace("%s", parent.currentImage - 1);
                     filenamebackground = mapbackground_pattern.replace("%s", parent.currentImage - 1);
                     if (parent.currentImage == 1)
                         parent.round_robin_count = parent.round_robin_count + 1
                     if (parent.round_robin_count == 1){
                         repeat = false;
                         running = false;
                     }
            }
        }
    }
    Component.onCompleted: {
        filename = map_pattern.replace("%s", (count_of_maps - 1));
        filenamebackground = mapbackground_pattern.replace("%s", (count_of_maps - 1));
        //console.log("Map " + filenamebackground );
    }

}

