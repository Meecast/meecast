import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: about
    property int screen_width  :  768
    property int screen_height : 1280    

   

    content: 
    Container{
       id: absoluteLayoutContainer
//       background: Color.create("#f618df") 
       background: Color.create("#a61096") 

      // minHeight: 4000
       layout: AbsoluteLayout {}
       
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            background: Color.Black
            preferredWidth: 768
            preferredHeight: 120
       }                       
       ImageView {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 120
                 }
                imageSource: "asset:///share/images/mask_background.png"
                preferredWidth: 768  
        }
        ImageView {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 350
                 }
                imageSource: "asset:///share/images/mask_title.png"
                preferredWidth: 768 
                preferredHeight: 72*1.6
        }

        Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 460
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
                layout: AbsoluteLayout {}
                ImageView {
                   layoutProperties: AbsoluteLayoutProperties {
                       positionX: 768/2 - 64*1.6
                       positionY: 60
                   }
                   preferredWidth: 128*1.6
                   preferredHeight: 128*1.6
                   imageSource: Config.iconspath + "/" + Config.iconset + "/28.png"
                   horizontalAlignment: HorizontalAlignment.Center                
               } 
               Label {
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 0
                        positionY: 290
                    }
                    preferredWidth: 768
                    horizontalAlignment: HorizontalAlignment.Center                
                    id: versiontext
                    text: "Version" + " " + Config.version 
                    textStyle.textAlign: TextAlign.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.BigText
                        color: Color.White
                    }
                }   
                Container {
                //    layout: DockLayout {}
                    layoutProperties: AbsoluteLayoutProperties {
                            positionX: 0
                            positionY: 360
                    }
 
                    Label {
                        id: abouttext 
                        text: Config.tr("About") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                    }
                    Label {
                        multiline: true
                        id: aboutfulltext 
                        text: Config.tr("MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright (C) 2006-2011")
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }
                    Label {
                        id: projectsitetext 
                        text: Config.tr("Project website") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                        multiline: true
                        id: projectfulltext 
	                    text: "http://meecast.com"
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }
                    Divider {
                        horizontalAlignment: HorizontalAlignment.Fill
                    }
                    Label {
                        id: projectadministratortext 
                        text: Config.tr("Project administrator") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                        id: prohectadministatorfulltext 
	                    text: "Vlad Vasilyeu"
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
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
            text: Config.tr("MeeCast for Harmattan")
            textStyle.textAlign: TextAlign.Center
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
            }
        }
    }
}

