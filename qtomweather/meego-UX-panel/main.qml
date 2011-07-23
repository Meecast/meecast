import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as UX
//import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Window {
    id: scene
    showsearch: false
    title: "OMWeather Settings"
    applicationPage: ApplicationPage {
        id: appPage
        title: "OMWeather Settings"

        Item {
            id: main
            parent: appPage.content
            anchors.fill: parent
            Text {
                id: txt
                font.pointSize: 18
                anchors.top: parent.top
                anchors.left: parent.left
                text: "aaaaaaaaaaaaaaaa"
            }


            UX.DropDown {
                anchors.right: parent.right
                anchors.rightMargin: 10
                //selectedValue: "Meecast"
                model: ["Meecast", "Glance", "Other"]
                payload: ["meecast", "glance", "other"]
                width: 300
                titleColor: black
                replaceDropDownTitle: true
                onTriggered: {
                    txt.text = payload[index] + " = " + data + " - " + model[index];
                }
            }

        }
    }
}



