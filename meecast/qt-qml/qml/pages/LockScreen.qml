import Qt 4.7
import com.nokia.meego 1.0

Page {
    id: standbysettingspage
    property int margin: 16
    property int x_position: Config.lock_screen_x_position
    property int y_position: Config.lock_screen_y_position
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                Config.set_lock_screen_x_position(xSlider.value)
                Config.set_lock_screen_y_position(ySlider.value)
                pageStack.pop();
            }
        }
    }

    orientationLock: PageOrientation.LockPortrait

    Rectangle{
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 120
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
    }

    Rectangle {
        id: title_rect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 70
        color: "black"
        Label {
            id: title
            anchors.fill: parent
            color: "white"
            text: Config.tr("Settings for Lockscreen widget")
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: title_of_x_position
        anchors.top: title_rect.bottom
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 40
        color: "transparent"
        Label {
            id: label_of_x
            anchors.fill: parent
            color: "white"
            text: Config.tr("Select the X-position for left corner of widget")
            font.pixelSize: 20
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        id: box_x_position
        height: 60
        width: parent.width 
        anchors.top: title_of_x_position.bottom
        anchors.left: parent.left
        anchors.topMargin: margin 
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "transparent"
        Slider {
             id:xSlider
             anchors.fill: parent
             stepSize:1
             valueIndicatorVisible: true
             minimumValue: 0
             maximumValue: 480
             width: parent.width 
             value: x_position
         }
   }
    Rectangle {
        id: title_y_position
        anchors.top: box_x_position.bottom
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 30
        color: "transparent"
        Label {
            id: title_temperature_color
            anchors.fill: parent
            color: "white"
            text: Config.tr("Select the Y-position for left corner of widget")
            font.pixelSize: 20
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }
    Rectangle {
        id: box_y_position
        height: 60
        width: parent.width 
        anchors.top: title_y_position.bottom
        anchors.left: parent.left
        anchors.topMargin: margin 
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "transparent"
        Slider {
             id: ySlider
             anchors.fill: parent
             stepSize:1
             valueIndicatorVisible: true
             minimumValue: 0
             maximumValue: 854 
             width: parent.width 
             value: y_position
         }
   }


}


