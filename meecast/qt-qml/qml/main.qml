import bb.cascades 1.0

NavigationPane {
    id: rootWindow

 // Create the initial screen
    Page {
        content: Container {
            Label {
                text: "Initial page"
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
            },
            ImagePaintDefinition {
            	id: backgroundPaint
            	imageSource: "asset:///images/background.png"
            }
        ]

    } // end of Page
}
