import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: units
    property int margin: 16

    function getIndex(model, value){
        var i=0;
        console.log(model.count);
        console.log(model[0])
        console.log(value)
        while (i<model.count && model[i]!=value) i++;
        if (i == model.count) return -1;
        else return i;
    }

/*
    MySelectionDialog {
        id: temperature_dlg
        model: Config.temperature_list()
        titleText: Config.tr("Temperature units")
        selectedIndex: -1
        onAccepted: {
            Config.temperature_unit(selectedText);
        }
    }
    MySelectionDialog {
        id: wind_dlg
        model: Config.windspeed_list()
        titleText: Config.tr("Wind speed units")
        selectedIndex: -1
        onAccepted: {
            Config.windspeed_unit(selectedIndex);
        }
    }
    MySelectionDialog {
        id: pressure_dlg
        model: Config.pressure_list()
        titleText: Config.tr("Pressure units")
        selectedIndex: -1
        onAccepted: {
            Config.pressure_unit(selectedIndex);
        }
	}
	MySelectionDialog {
        id: visible_dlg
        model: Config.visible_list()
        titleText: Config.tr("Visible units")
        selectedIndex: -1
        onAccepted: {
            Config.visible_unit(selectedIndex);
        }
    }
    */
     PageHeader {
                title: Config.tr("Measurement units")
            }


    Rectangle{
        anchors.fill: parent
//        anchors.top: title_rect.bottom
        anchors.topMargin: 80
//        anchors.leftMargin: margin
//        anchors.rightMargin: margin

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
//		SilicaListView {
		SilicaFlickable {
			anchors.fill: parent
//		    flickableDirection: Flickable.VerticalFlick
//            header: PageHeader {
//                title: Config.tr("Measurement units")
//            }

		    contentHeight: 850 

//            model: VisualItemModel {
		    Column {
				anchors.fill: parent
				//spacing: 20
                ComboBox {
                    label: Config.tr("Temperature units")
                    currentIndex: -1
                    width: units.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("Celsius")
                            onClicked: { Config.temperature_unit("C") } 
                        }
                        MenuItem {
                            text: Config.tr("Fahrenheit")
                            onClicked: { Config.temperature_unit("F") }
                        }
                    }
    				Component.onCompleted: {
                        for (var i=0; i<Config.temperature_list().length; i++) {
                            if  (Config.temperature_list()[i] == Config.temperatureunit)
                                currentIndex = i 
                        }
				    }
                }

                ComboBox {
                    label: Config.tr("Wind speed units")
                    currentIndex: -1
                    width: units.width
                    menu: ContextMenu {
                        MenuItem { 
                            text: Config.tr("m/s")
                            onClicked: { Config.windspeed_unit("0") } 
                        }
                        MenuItem {
                            text: Config.tr("km/h")
                            onClicked: { Config.windspeed_unit("1") } 
                        }
                        MenuItem {
                            text: Config.tr("mi/h")
                            onClicked: { Config.windspeed_unit("2") } 
                        }
                        MenuItem {
                            text: Config.tr("Knots")
                            onClicked: { Config.windspeed_unit("3") } 
                        }
                        MenuItem {
                            text: Config.tr("Beaufort scale")
                            onClicked: { Config.windspeed_unit("4") } 
                        }

                    }
    				Component.onCompleted: {
                        for (var i=0; i<Config.windspeed_list().length; i++) {
                            if  (Config.windspeed_list()[i] == Config.windspeedunit)
                                currentIndex = i 
                        }
				    }
                }

				Label {
					text: Config.tr("Pressure units")
					height: 60
					horizontalAlignment: Text.AlignLeft
					verticalAlignment: Text.AlignVCenter
				}
                /*
				ButtonColumn {
					width: parent.width
					platformStyle: ButtonStyle {
						horizontalAlignment: Text.AlignLeft
					}
					Button {
						text: Config.tr("mbar")
						checked: (Config.pressureunit == "mbar")
						onClicked: {
							Config.pressure_unit("0");
						}
					}
					Button {
						text: Config.tr("Pa")
						checked: (Config.pressureunit == "Pa")
						onClicked: {
							Config.pressure_unit("1");
						}
					}
					Button {
						text: Config.tr("mmHg")
						checked: (Config.pressureunit == "mmHg")
						onClicked: {
							Config.pressure_unit("2");
						}
					}
				}
                */
				Label {
					text: Config.tr("Visible units")
					height: 60
					horizontalAlignment: Text.AlignLeft
					verticalAlignment: Text.AlignVCenter
				}
                /*
				ButtonColumn {
					width: parent.width
					platformStyle: ButtonStyle {
						horizontalAlignment: Text.AlignLeft
					}
					Button {
						text: Config.tr("m")
						checked: (Config.visibleunit == "m")
						onClicked: {
							Config.visible_unit("0");
						}
					}
					Button {
						text: Config.tr("km")
						checked: (Config.visibleunit == "km")
						onClicked: {
							Config.visible_unit("1");
						}
					}
					Button {
						text: Config.tr("mi")
						checked: (Config.visibleeunit == "mi")
						onClicked: {
							Config.visible_unit("2");
						}
					}

				}
                */

				/*
			Button {
				anchors.horizontalCenter: parent.horizontalCenter
				text: Config.tr("Wind speed")+": "+Config.windspeedunit
				onClicked: {
					wind_dlg.open();
				}
				Component.onCompleted: {
					wind_dlg.selectedIndex = units.getIndex(wind_dlg.model, Config.windspeedunit)
				}
			}*/
            }
//			}
		}
    }
}
