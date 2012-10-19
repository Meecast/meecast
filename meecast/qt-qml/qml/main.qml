import bb.cascades 1.0

NavigationPane {
    id: rootWindow

 // Create the initial screen
    Page {
        property int screen_width : 1280
        property int screen_height : 768
        
        content: Container {
            background: Color.White
//            layout: StackLayout {}

            layout: AbsoluteLayout {}
            //horizontalAlignment: HorizontalAlignment.Center
            //verticalAlignment: VerticalAlignment.Center
            preferredWidth: screen_width
            preferredHeight: screen_height
			Container{
                layoutProperties: AbsoluteLayoutProperties {
                	positionX: 0
                    positionY: 0
                }
                background: Color.Black
                preferredWidth: 768
                preferredHeight: 100
        	}                       
       		ImageView {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 100
                }
                imageSource: "asset:///images/mask_background.png"
                preferredWidth: 768  
        	}
        	Container{
                layoutProperties: AbsoluteLayoutProperties {
                	positionX: 0
                    positionY: 360
                }
                background: Color.Black
                preferredWidth: 768
                preferredHeight: 1000
        	}
			Container{
                preferredWidth: 768
		        layout: DockLayout {}
            
                layoutProperties: AbsoluteLayoutProperties {
                	positionX: 0
                    positionY: 0
                }
                background: Color.Black
				Label {                 
				   text: "<"
				   horizontalAlignment: HorizontalAlignment.Left
					textStyle {
					   base: SystemDefaults.TextStyles.BigText
					   color: Color.White
					 }
			    }
			    Container{
			        layout: DockLayout {}
			        preferredWidth: 600 
			        horizontalAlignment: HorizontalAlignment.Center 
				    Label {                 
				        text: Config.stationname
				        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
					        base: SystemDefaults.TextStyles.BigText
					        color: Color.White
				        }
				    }
                }
				Label {                 
				   text: ">"
				   horizontalAlignment: HorizontalAlignment.Right
	               textStyle {
					   base: SystemDefaults.TextStyles.BigText
					   color: Color.White
				   }
			    }
        	}
        }
         
        actions: [
            // Create the "Push" action
            ActionItem {
                title: "Setting"
                //ActionBar.placement: ActionBarPlacement.OnBar
                ActionBar.placement: ActionBarPlacement.InOverflow
                 
                // When this action is selected, create an object that's based
                // on the ComponentDefinition below, and then push it on to
                // the stack to display it
                onTriggered: {
                    var newPage = settingspageDefinition.createObject();
                    rootWindow.push(newPage);
                }
            },
	    ActionItem {
                title: "About"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    var newPage = aboutpageDefinition.createObject();
                    navigationPane.push(newPage);
                }
            },
            ActionItem {
                title: "Refresh"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    Config.updatestations()	
                }
            }
        ]
         
        attachedObjects: [
            ComponentDefinition {
                id: settingspageDefinition
                source: "SettingsPage.qml"
            },
	        ComponentDefinition {
                id: sboutpageDefinition
                source: "AboutPage.qml"
            }
        ]

    } // end of Page
}
