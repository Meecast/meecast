import Qt 4.7
import MeeGo.Labs.Components 0.1
import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

ApplicationPage {
    id: page
    title: qsTr("OMWeather Settings")

    Item {
        anchors.fill: page.content

        Flickable {
            contentHeight: contents.height
            anchors.fill: parent
            clip: true

            Column {
                id: contents
                width: parent.width

                Image {
                    id: iconsetItem
                    source: "image://theme/pulldown_box"
                    width: parent.width

                    Text {
                        id: txt
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        text: qsTr("iconset:")
                        width: 100
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                    }

                    DropDown {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        //selectedValue: "Meecast"
                        model: ["Meecast", "Glance", "Other"]
                        payload: ["meecast", "glance", "other"]
                        width: 300

                        onTriggered: {
                            txt.text = payload[index] + " = " + data + " - " + model[index];
                        }
                    }
                }
            }
        }
    }
}
