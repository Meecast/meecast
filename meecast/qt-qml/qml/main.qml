import bb.cascades 1.0

NavigationPane {
    id: rootWindow

 // Create the initial screen
    Page {
        property int screen_width : 1280
        property int screen_height : 768
        
        content: Container {
            background: Color.Black
            layout: StackLayout {}
            //horizontalAlignment: HorizontalAlignment.Center
            //verticalAlignment: VerticalAlignment.Center
            preferredWidth: screen_width
            preferredHeight: screen_height
            opacity: 1.0
            Container {
                id: absoluteLayoutContainer
                
                background: Color.create ("#000000")
                layout: AbsoluteLayout {}
                preferredWidth: screen_width
                preferredHeight: screen_height
                ImageView {
                    layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                    }
                 imageSource: "asset:///images/mask_background.png"
                 
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
                    var newPage = pageDefinition.createObject();
                    navigationPane.push(newPage);
                }
            },
	    ActionItem {
                title: "About"
                ActionBar.placement: ActionBarPlacement.InOverflow
                onTriggered: {
                    var newPage = settingspageDefinition.createObject();
                    navigationPane.push(newPage);
                }
            },
            ActionItem {
                title: "Refresh"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                      var newPage = aboutpageDefinition.createObject();
                      navigationPane.push(newPage);
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
