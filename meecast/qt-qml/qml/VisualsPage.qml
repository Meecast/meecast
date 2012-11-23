import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: visuals 
    property int screen_width  :  768
    property int screen_height : 1280    


	content: 
	Container{
	    id: absoluteLayoutContainer
        background: Color.White

        layout: AbsoluteLayout {}
        attachedObjects: [
       	    ComponentDefinition {
       		   id: nextpage6
       		}
        ]
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
				Container {
                    layout: DockLayout {}
                    Label {                 
                          text: Config.tr("Iconset")+": "+Config.iconset 
                          preferredWidth: 768
                          horizontalAlignment:  HorizontalAlignment.Fill
                          verticalAlignment: VerticalAlignment.Center
                          textStyle {
                              base: SystemDefaults.TextStyles.TitleText
                              color: Color.White
                          }
                      }
                      Label {                 
                        text: ">"
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                            color: Color.White
                        }
                      }
				}
				onTouch: {
                    if (event.isDown()) {
                        nextpage6.source = "IconsetPage.qml"
                        var newPage = nextpage6.createObject();
                        rootWindow.push(newPage);
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
            text: Config.tr("Appearance")
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
            }
        }
    }
}

