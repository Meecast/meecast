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
                                settings2_absoluteLayoutContainer.update_list()
                                rootWindow.navigateTo(settings2);
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
                                settings2_absoluteLayoutContainer.update_list()
                                rootWindow.navigateTo(settings2);
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
                                settings2_absoluteLayoutContainer.update_list()
                                rootWindow.navigateTo(settings2);
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

