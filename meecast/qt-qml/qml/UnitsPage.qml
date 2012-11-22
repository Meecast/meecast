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
                                positionY: 0
            }
			attachedObjects: [
				ComponentDefinition {
					id: nextpage
				}
			]
	
    layout: StackLayout {
		                
		                }
           Label {
			                    text: "Filling"
		                    bottomMargin: 9
							textStyle {
								base: SystemDefaults.TextStyles.SmallText
								color: Color.White
							}

		  }
																					                   			RadioGroup {
		        id: lasagnaOption
		            Option {
	                    text: Config.tr("Celsius")
						selected: (Config.temperatureunit == 'C') 
		            }
		            Option {
		                text: Config.tr("Fahrenheit")
						selected: (Config.temperatureunit == 'F') 
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
				base: SystemDefaults.TextStyles.TitleText
				color: Color.White
			}
		}
	}
	attachedObjects: [
        TextStyleDefinition {
            id: textStyleLasangaPlate
            base: SystemDefaults.TextStyles.TitleText
            color: Color.create ("#88000000")
            lineHeight: 0.85
        }
    ]

}

