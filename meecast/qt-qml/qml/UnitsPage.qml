import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: units 
    property int screen_width  :  768
    property int screen_height : 1280    

   

    content:  Container{
        
       id: absoluteLayoutContainer
       background: Color.White
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
		Container{
			layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 90
            }
	
            layout: StackLayout {}
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
		}
		Container{
			layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 480
            }
	
            layout: StackLayout {}
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

