import Qt 4.7
//import Qt.labs.components 1.0
import com.meego 1.0
import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

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
    Config {id: config}
    Item {
        id: mainitem

        MySelectionDialog {
            id: temperature_dlg
            titleText: "Temperature Units"
            selectedIndex: config.TemperatureUnit()
            /*model: ListModel {
                ListElement {name: "C"; key: "c"}
                ListElement {name: "F"; key: "f"}
            }*/
            model: config.Stations()
            //model: ["C", "F"]
        }

        //parent: appPage.content
        anchors.fill: parent
        Column {
            width: parent.width

            Button {
                text: temperature_dlg.selectedIndex >= 0 ?
                          "Temperature Units: " + temperature_dlg.model.get(temperature_dlg.selectedIndex).name :
                          "Temperature Units"
                onClicked: {
                    temperature_dlg.open();
                }
            }

        }
    }
}
