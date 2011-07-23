import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as UX
import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Window {
    id: scene
    showsearch: false
    title: "OMWeather Settings"

    applicationPage: ApplicationPage {
        id: appPage
        title: "OMWeather Settings"

        Item {
            id: main
            Config {id: config}
            parent: appPage.content
            anchors.fill: parent
            Column {
                width: parent.width


            Image {
                source: "image://theme/pulldown_box"
                width: parent


                Text {
                    id: txt
                    font.pointSize: 18
                    anchors.top: parent.top
                    anchors.left: parent.left
                    text: "IconSet"
                }
                UX.DropDown {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    title: config.iconSet()
                    model: config.icons()
                    //model: ["meecast", "glance", "other"]
                    width: 300
                    titleColor: black
                    replaceDropDownTitle: true
                    onTriggered: {
                        //txt.text = payload[index] + " = " + data + " - " + model[index];
                    }
                }
            }
            Image {
                source: "image://theme/pulldown_box"
                width: parent

                Text {
                    id: txt_updateperiod
                    font.pointSize: 18
                    anchors.top: parent.top
                    anchors.left: parent.left
                    text: "Ipdate Period"
                }
                UX.DropDown {
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    title: config.UpdatePeriod()
                    model: config.UpdatePeriodValues()
                    payload: config.UpdatePeriodKeys()
                    width: 300
                    titleColor: black
                    replaceDropDownTitle: true
                    onTriggered: {
                        //txt.text = payload[index] + " = " + data + " - " + model[index];
                    }
                }
            }
            }
        }
    }
}



