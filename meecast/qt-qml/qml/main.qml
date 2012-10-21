import bb.cascades 1.0

NavigationPane {
    

    id: rootWindow

 // Create the initial screen
    Page {
        property int screen_width : 1280
        property int screen_height : 768

        id: main
        function updatestationname(){
			main.updatemodels();
			stationname.text = Config.stationname;
			left_arrow.visible = Config.prevstationname == "" ? false : true;
			right_arrow.visible = Config.nextstationname == "" ? false : true;
			sourceicon.visible = false;
			sourceicon.source = Config.imagespath + "/" + Config.source + ".png";
			sourceicon.visible = true;
        }


        function onConfigChanged() {
            console.log("end update station name = "+Config.stationname);
//            startview.visible = Config.stationname == "Unknown" ? true : false;
//            mainview.visible = Config.stationname == "Unknown" ? false : true;
            main.updatestationname();
            isUpdate = false;
        }

        function updatemodels()
        {
			Current.reload_data(Config.filename);
			Current.update_model(0);
			Current_night.update_model(1);
			Forecast_model.update_model(2);
			Forecast_night_model.update_model(3);
			Forecast_hours_model.update_model(4);
			//list.height = 80 * Forecast_model.rowCount();
			console.debug ("Forecast_model.rowCount()", Forecast_model.rowCount(), Current.rowCount());
			dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
			current_rect.visible = Current.rowCount() == 0 ? false : true;
			//list.visible = (Forecast_model.rowCount() == 0) ? false : true;
        }
   
        content: Container {
            background: Color.White

            onCreationCompleted: {
                 Config.configChanged.connect (main.onConfigChanged);
                 Current.update_model(0);
            }
            layout: AbsoluteLayout {}
            Container{
                id: current_rect
//            	visible: Current.rowCount() == 0 ? false : true
            	background: Color.Red
            	preferredWidth: 768
            	preferredHeight: 290
                layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0
                                positionY: 95
                }
                layout: AbsoluteLayout {
                }
                ImageView {
                	imageSource: "asset:///images/mask_background_main.png"
                    preferredWidth: 768
                    layoutProperties: AbsoluteLayoutProperties {
                    }
                }
                Container{
                    preferredWidth: 768
                	layout: DockLayout {}
                	layoutProperties: AbsoluteLayoutProperties {
                	    positionY: 75.0
                	}
                	Label {                 
                	    text: Current.getdata(0, "description");
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
					       base: SystemDefaults.TextStyles.BodyText
					       color: Color.White
					    }
			        }
			    }

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
