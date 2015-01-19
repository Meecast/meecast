import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: lockScreen
    Item{
        id: weatherData
        property QtObject meecastData 
        property bool active: true
        visible: true
        Component.onCompleted: {
            meecastData = Qt.createQmlObject('import QtQuick 2.0; import org.meecast.data 1.0 as Meecast; Meecast.Data {}', weatherData)
        }
        anchors {
            topMargin: Theme.paddingMedium
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width  
        Label {
            id: mylabel
            text: weatherData.meecastData.nameString 
        }
    Connections {
        target: weatherData.meecastData 
        onRefreshWidget: {            
           console.log("sssssssssssssssssssssssssssss")
           mylabel.text = weatherData.meecastData.nameString 
        }
    }

    }

}
