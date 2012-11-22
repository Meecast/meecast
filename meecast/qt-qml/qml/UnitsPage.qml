import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: units 
    property int screen_width  :  768
    property int screen_height : 1280    

   

	content: 
	Container{
		id: absoluteLayoutContainer
       background: Color.Black

      // minHeight: 4000
       layout: AbsoluteLayout {}
       
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            background: Color.Black
            preferredWidth: 768
            preferredHeight: 90
       }                       
       ImageView {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 90
                 }
                imageSource: "asset:///share/images/mask_background_grid.png"
                preferredWidth: 768  
        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
	                positionX: 0
                    positionY: 350
                }
                background: Color.Black
                preferredWidth: 768
                preferredHeight: 1000
        }
              
        ScrollView {
			layoutProperties: 
            AbsoluteLayoutProperties {
            	positionY: 90
            }
            preferredHeight: 1050.0
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }
		   	Container{
			    Label {
                    text: Config.tr("Temperature units") 
                    bottomMargin: 9
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.create("#999999") 
                    }
                }
                RadioGroup {
                    id: temperatureOption
                    dividersVisible: false 
                    Option {
                        text: Config.tr("Celsius")
                        selected: (Config.temperatureunit == 'C') 
                        value: "C"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.temperature_unit('C');
                            }
                        }
                     }
                     Option {
                        text: Config.tr("Fahrenheit")
                        selected: (Config.temperatureunit == 'F') 
                        value: "F"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.temperature_unit('F');
                            }
                        }
                     }
                } 
                Divider {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                Label {
                    text: Config.tr("Wind speed units") 
                    bottomMargin: 9
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.create("#999999") 
                    }
                }
                RadioGroup {
                    id: windspeedOption
                    Option {
                        text: Config.tr("m/s")
                        selected: (Config.windspeedunit == 'm/s') 
                        value: "m/s"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.windspeed_unit("0");
                            }
                        }
                    }
                    Option {
                        text: Config.tr("km/h")
                        selected: (Config.windspeedunit == 'km/h') 
                        value: "km/h"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.windspeed_unit("1");
                            }
                        }
                     }

                     Option {
                        text: Config.tr("mi/h")
                        selected: (Config.windspeedunit == 'mi/h') 
                        value: "mi/h"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.windspeed_unit("2");
                            }
                        }
                     }
                } 
                Divider {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                Label {
                    text: Config.tr("Pressure units") 
                    bottomMargin: 9
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.create("#999999") 
                    }
                }
                RadioGroup {
                    id: pressureOption
                    Option {
                        text: Config.tr("mbar")
                        selected: (Config.pressureunit == "mbar") 
                        value: "mbar"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.pressure_unit("0");
                            }
                        }
                    }
                    Option {
                        text: Config.tr("Pa")
                        selected: (Config.pressureunit == "Pa") 
                        value: "Pa"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.pressure_unit("1");
                            }
                        }
                     }

                     Option {
                        text: Config.tr("mmHg")
                        selected: (Config.pressureunit == 'mmHg') 
                        value: "mmHg"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.pressure_unit("2");
                            }
                        }
                     }
                } 
                Divider {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
                Label {
                    text: Config.tr("Visible units") 
                    bottomMargin: 9
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.create("#999999") 
                    }
                }
                RadioGroup {
                    id: visibleOption
                    Option {
                        text: Config.tr("m")
                        selected: (Config.visibleunit == "m") 
                        value: "m"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.visible_unit("0");
                            }
                        }
                    }
                    Option{ 
                        text: Config.tr("km")
                        selected: (Config.visibleunit == "km") 
                        value: "km"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.visble_unit("1");
                            }
                        }
                     }
                     Option {
                        text: Config.tr("mi")
                        selected: (Config.visibleunit == "mi") 
                        value: "mi"
                        onSelectedChanged: {
                            if (selected == true) {
                                Config.visible_unit("2");
                            }
                        }
                     }
                } 
                Divider {
                    horizontalAlignment: HorizontalAlignment.Fill
                }
            }
        }
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredWidth: 768
            text: Config.tr("Measurement units") 
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
            }
        }
    }
}

