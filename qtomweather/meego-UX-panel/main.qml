import Qt 4.7
import MeeGo.Labs.Components 0.1
import MeeGo.Components 0.1 as UX
import "/opt/com.meecast.omweather/lib/OmweatherPlugin" 0.1

Window {
    id: scene
    showsearch: false
    title: "OMWeather Settings"

    Component.onCompleted: {
        applicationPage = appPageComponent
    }
    Config {id: config}
    Component {
        id: appPageComponent
        ApplicationPage {
            id: appPage
            title: "OMWeather Settings"

            Item {
                id: main

                parent: appPage.content
                anchors.fill: parent
                Column {
                    width: parent.width


                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width
                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Temperature Units")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.TemperatureUnit()
                            model: config.TemperatureUnits()
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                config.TemperatureUnit(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Wind speed Units")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.WindSpeedUnit()
                            model: config.WindSpeedUnits()
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                config.WindSpeedUnit(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("IconSet")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.iconSet()
                            model: config.icons()
                            //model: ["meecast", "glance", "other"]
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                //txt.text = payload[index] + " = " + data + " - " + model[index];
                                config.iconSet(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Update interval")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        UX.DropDown {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            title: config.UpdatePeriod()
                            model: config.UpdatePeriods()
                            width: 300
                            titleColor: black
                            replaceDropDownTitle: true
                            onTriggered: {
                                //txt.text = payload[index] + " = " + data + " - " + model[index];
                                config.UpdatePeriod(model[index])
                            }
                        }
                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Auto-update on connection")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }

                        UX.ToggleButton {
                            on: config.UpdateConnect()
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 10

                            onToggled: {
                                config.UpdateConnect(isOn);
                            }
                        }

                    }
                    Image {
                        source: "image://theme/pulldown_box"
                        width: parent.width

                        Text {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            text: qsTr("Stations") + " >"
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                appPage.addApplicationPage(stationsPageComponent);
                            }
                        }

                    }

                    UX.Button{
                        height: 40
                        text: qsTr("Save")
                        onClicked: {
                            config.saveConfig();
                        }
                    }
                }
            }
        }
    }
    Component {
        id: stationsPageComponent
        ApplicationPage {
            id: stationsPage
	    title: qsTr("OMWeather Stations Settings")
            Item {
                parent: stationsPage.content
                anchors.fill: parent
                ListView {
		    id: stationlist
		    height: parent.height
                    width: parent.width
                    model: config.Stations()
                    delegate:
                        Image {
                            source: "image://theme/pulldown_box"
                            width: parent.width

                            Text {
				id: txt
                                //anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }

                            UX.Button {
				id: but
				height: 40
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                text: qsTr("Delete")
                                onClicked: {
					but.text = modelData
                                }
                            }

                        }

                }
            }
        }
    }
}



