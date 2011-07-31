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
    Updater {id: updater}
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
                            titleColor: "black"
                            replaceDropDownTitle: true
                            onTriggered: {
                                //updater.makeQmlData(); /* for debug omweather plugin */
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
                            titleColor: "black"
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
                            titleColor: "black"
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
                            titleColor: "black"
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
                            text: qsTr("Stations")
                            width: 100
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                        }
                        Image {
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.margins: 10
                            source: "image://theme/settings/pulldown_arrow_up"
                            transform: Rotation {angle: 90}
                        }
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                appPage.addApplicationPage(stationsPageComponent);
                            }
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

            menuContent: UX.ActionMenu {
                id: actions
                model: [qsTr("Add new station")]
                onTriggered: {
                    newstation.show();
                    stationsPage.closeMenu();
                }
            }

            UX.ModalDialog {
                id: newstation
                title: qsTr("New Station")
                showAcceptButton: true
                showCancelButton: true
                acceptButtonText: qsTr("OK")
                cancelButtonText: qsTr("Cancel")
                width: 800
                height: 6 * 77
                content: Item {
                    width: 800
                    height: 6 * 77
                    Column {
                        width: parent.width


                        Image {
                            source: "image://theme/pulldown_box"
                            width: parent.width
                            Text {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: qsTr("Sources")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: source
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.TemperatureUnit()
                                model: config.Sources()
                                width: 500
                                titleColor: "black"
                                replaceDropDownTitle: true
                                onTriggered: {
                                    country.model = config.Countries(model[index], false);
                                    country.payload = config.Countries(model[index], true);
                                    country.selectedIndex = -1;
                                    country.selectedTitle = "";
                                    region.selectedIndex = -1;
                                    region.selectedTitle = "";
                                    region.model = "";
                                    city.selectedIndex = -1;
                                    city.selectedTitle = "";
                                    city.model = "";
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
                                text: qsTr("Countries")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: country
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.WindSpeedUnit()
                                model: config.Countries(source.title, false)
                                payload: config.Countries(source.title, true)
                                width: 500
                                titleColor: "black"
                                replaceDropDownTitle: true
                                onTriggered: {
                                    region.model = config.Regions(payload[index], false);
                                    region.payload = config.Regions(payload[index], true);
                                    region.selectedIndex = -1;
                                    region.selectedTitle = "";
                                    city.selectedIndex = -1;
                                    city.selectedTitle = "";
                                    city.model = "";
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
                                text: qsTr("Region")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: region
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.WindSpeedUnit()
                                //model: config.Regions(country.payload, false)
                                //payload: config.Regions(source.payload, true)
                                width: 500
                                titleColor: "black"
                                replaceDropDownTitle: true
                                onTriggered: {
                                    city.model = config.Cities(payload[index], false);
                                    city.payload = config.Cities(payload[index], true);
                                    city.selectedIndex = -1;
                                    city.selectedTitle = "";
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
                                text: qsTr("City")
                                width: 100
                                height: parent.height
                                verticalAlignment: Text.AlignVCenter
                            }
                            UX.DropDown {
                                id: city
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                //title: config.WindSpeedUnit()
                                //model: config.Regions(country.payload, false)
                                //payload: config.Regions(source.payload, true)
                                width: 500
                                titleColor: "black"
                                replaceDropDownTitle: true
                                onTriggered: {

                                }
                            }
                        }
                    }

                }
                onAccepted: {
                    //console.log(source.selectedIndex + " " + source.selectedTitle);
                    //console.log(city.selectedIndex + " " + city.selectedTitle + " " + city.payload[city.selectedIndex]);
                    config.saveStation(city.payload[city.selectedIndex],
                                       city.selectedTitle,
                                       region.selectedTitle,
                                       country.selectedTitle,
                                       source.selectedTitle,
                                       source.selectedIndex);
                    stationlist.model = config.Stations();
                }
                onRejected: {
                }
            }

            Item {
                parent: stationsPage.content
                anchors.fill: parent
                ListView {
		    id: stationlist
                    height: config.StationsCount() * 77 + 5
                    width: parent.width
                    model: config.Stations()
                    delegate:
                        Image {
                            id: img
                            source: "image://theme/pulldown_box"
                            width: parent.width

                            Text {
				id: txt
                                //anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: model.modelData.name
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
                                    config.removeStation(model.modelData.id);
                                    stationlist.model = config.Stations();
                                    stationlist.height = config.StationsCount() * 77 + 5;
                                }
                            }

                        }

                }

            }
        }
    }
}



