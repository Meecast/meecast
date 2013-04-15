import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: about
    property int screen_height : Config.get_height_resolution() 
    property int screen_width : Config.get_width_resolution() 
    property int menu_height : Config.get_height_resolution() == 1280 ? 138 : 110;


    content: 
    Container{
       id: absoluteLayoutContainer
       background: Color.create("#262626") 
       layout: AbsoluteLayout {}
              
        ScrollView {
            layoutProperties: 
            AbsoluteLayoutProperties {
                positionY: 0
            }
            preferredHeight: screen_height - menu_height 
            scrollViewProperties {
                scrollMode: ScrollMode.Vertical
            }

            Container{
                layout: AbsoluteLayout {}
                Container{
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 0
                        positionY: 0
                    }
                    background: Color.Black
                    preferredWidth: screen_width
                    preferredHeight: 120
               }                       
        //       ImageView {
        //                layoutProperties: AbsoluteLayoutProperties {
        //                    positionX: 0
        //                    positionY: 120
        //                 }
        //                imageSource: "asset:///share/images/mask_background.png"
        //                preferredWidth: 768  
        //        }
                ImageView {
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 0
                            positionY: 260
                         }
                        imageSource: "asset:///share/images/mask_title.png"
                        preferredWidth: screen_width 
                        preferredHeight: 72*1.6
                }

                Container{
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 0
                            positionY: 460
                        }
                        background: Color.create("#262626") 
                        //background: Color.Black
                        preferredWidth: screen_width
                        preferredHeight: screen_height - 80 
                }
                Label {
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 0
                        positionY: 0
                    }
                    preferredWidth: screen_width
                    text: Config.tr("MeeCast for Blackberry 10")
                    textStyle.textAlign: TextAlign.Center
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
                        color: Color.White
                    }
                }

                ImageView {
                   layoutProperties: AbsoluteLayoutProperties {
                       positionX: screen_width/2 - 64*1.6
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
                        positionY: 260
                    }
                    preferredWidth: screen_width 
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
                            positionY: 380
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
                        text: Config.tr("MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright (C) 2006-2012")
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
                    Label {
                        id: leadprogrammertext 
                        text: Config.tr ("Programmers") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                        id: leadprogrammerfulltext 
	                    text: "Vlad Vasilyeu, Tanya Makava"
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }
                    Label {
                        id: leaddesignertext 
                        text: Config.tr("Lead designer") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                        id: leaddesignerfulltext 
	                    text: "Andrew Zhilin"
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }
                    Label {
                        id: projectmanagertext 
                        text: Config.tr("Project manager") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                        id: projectmanagerfulltext 
	                    text: "Ludmila Lisovskaya"
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }
                    Label {
                        id: translatorstext
                        text: Config.tr("Translators") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                        id: translatorsfulltext 
	    text: Config.tr ("\
French -  Nicolas Granziano, \n \
\tPhilippe, Benot Knecht \n \
Russian - Pavel Fialko, Vlad Vasiliev, \n \
\tEd Bartosh\n \
Finnish - Marko Vertainen\n \
German -  Claudius Henrichs\n \
Italian - Pavel Fialko, Alessandro Pasotti,\n \
\tSalvatore Fiorenzo, Ilir Gjika,\n \
\tLuca Onnis\n \
Spanish - Alejandro Lopez\n \
Polish - Tomasz Dominikowski\n \
Slovak - Roman Moravčík\n \
Dutch - Tom Leenders, Michiel Pater\n \
Bulgarian - Zlatko Enikov\n \
Albanian - Ilir Gjika\n \
Chinese - Wong Man Kong, Michael\n \
Portuguese - Jose Narciso,\n \
Turkish - Burak Kaan Kose,\n \
Catalan - Agustí Clara,\n \
Arabic - Tarad Alfandi,\n \
Norwegian - Peer-Atle Motland,\n \
Vietnamese - Hieu Nguyen\n \
\tAnh Dương and truongnx, \n \
Swedish - Angela Nennebrandt,\n \
\tNiklas Åkerström, \n \
\tTomislav Veleckovik")

                        multiline: true
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }

                    Label {
                            id: iconsetext  
                            text: Config.tr("Iconsets") + ":"
                            textStyle {
                                base: SystemDefaults.TextStyles.TitleText 
                                color: Color.create("#999999")
                            }
                        }
                	Label {
                            id: iconsetfulltext 
text: "Beginning - \n \
Beginning beginning_somehow(at)yahoo.com \n\n\
Contour - \n\
Glance  - \n\
MeeCast - \n\
Andrew Zhilin  http://tabletui.wordpress.com \n\n\
Epona   - \n\
Gianni Polito aka Epona http://crystalxp.net \n\n\
Grzankas  -\n\
Wojciech Grzanka http://http://grzanka.pl/ \n\n\
SimpelMee - \n\
Kim Venetvirta  tardolus(at)ovi.com \n\n\
Noun   - \n\
designed by Adam Whitcroft \n\
from The Noun Project. http://thenounproject.com\n\n\
Shiny - \n\
JyriK http://jyrik.deviantart.com \n\
jyrik(at)koti.luukku.com";
                            multiline: true
                            textStyle {
                                base: SystemDefaults.TextStyles.BodyText 
                                color: Color.White
                            }
	                }

                    Label {
                        id: licensetext  
                        text: Config.tr("License") + ":"
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText 
                            color: Color.create("#999999")
                         }
                     }
                     Label {
                         id: licensefulltext 
	    text: Config.tr ("This software is free software; you can\n \
redistribute it and/or modify it under \n \
the terms of the GNU General Public\n \
License as published by the Free Software \n \
Foundation; either version 2.1 of the \n \
License, or (at your option) any \n \
later version.");
                        multiline: true
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText 
                            color: Color.White
                        }
                    }
                }
            }
        }
    }
}





