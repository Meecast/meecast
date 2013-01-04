import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: units 
    property int screen_width  :  768
    property int screen_height : 1280    

   

    content: 
    Container{
        id: absoluteLayoutContainer
       background: Color.White

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
                                settings2_absoluteLayoutContainer.update_list()
                                rootWindow.navigateTo(settings2);
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
                                settings2_absoluteLayoutContainer.update_list()
                                rootWindow.navigateTo(settings2);
                            }
                        }
                     }
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

