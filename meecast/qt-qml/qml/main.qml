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
                
//            	visible: Current.rowCount() == 0 ? false : true
            	background: Color.Black
            	preferredWidth: 768
            	//preferredHeight: 290
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 95
                }
                layout: AbsoluteLayout {
                }
                Container{
                    id: current_rect
                    preferredWidth: 768
                    preferredHeight: 390
                    layoutProperties: AbsoluteLayoutProperties {
                        positionY: 30
                    }
                    background: Color.Red
                }
                ImageView {
                	imageSource: "asset:///images/mask_background_main.png"
                    preferredWidth: 768
                    layoutProperties: AbsoluteLayoutProperties {
                           positionY: 30
                    }
                }
                Container{
                    preferredWidth: 768
                    layout: DockLayout {}
                    layoutProperties: AbsoluteLayoutProperties {
                        positionY: 0.0
                    }
                    Label {                 
                        text: Current == true ? Config.tr("Now") : Config.tr("Today")
                        horizontalAlignment: HorizontalAlignment.Left
                        verticalAlignment: VerticalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                			color: Color.White
                		}
                	}
                	ImageView {
                	    imageSource: Config.iconspath + "/" + Config.iconset + "/" + Current.getdata(0, "pict")
                	    horizontalAlignment: HorizontalAlignment.Center                
                	    layoutProperties: AbsoluteLayoutProperties {
                	    }
                	}
                	
                	Label {                 
                	    id: temp_text
                        text: Current.getdata(0, "temp") + '°';
                        horizontalAlignment: HorizontalAlignment.Right
                        verticalAlignment: VerticalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                			color: Color.White
                		}
						function getColor(t)
                    	{
                            var c1, c2, c3;
                            if (Config.temperatureunit == "F"){
                                t = (t - 32) * 5 / 9;
                            }
                            if (t >= 30){
                                c2 = (t - 50)*(246/255-60/255)/(30-50) + 60/255;
                                return Qt.rgba(1, c2, 0, 1);
                            }else if (t < 30 && t >= 15){
                                c1 = (t - 30)*(114/255-1)/(15-30) + 1;
                                c2 = (t - 30)*(1-246/255)/(15-30) + 246/255;
                                return Qt.rgba(c1, c2, 0, 1);
                            }else if (t < 15 && t >= 0){
                                c1 = (t - 15)*(1-114/255)/(0-15) + 144/255;
                                c3 = (t - 15)*(1-0)/(0-15) + 0;
                                return Qt.rgba(c1, 1, c3, 1);
                            }else if (t < 0 && t >= -15){
                                c1 = (t - 0)*(0-1)/(-15-0) + 1;
                                c2 = (t - 0)*(216/255-1)/(-15-0) + 1;
                                return Qt.rgba(c1, c2, 1, 1);
                            }else if (t < -15 && t >= -30){
                                c2 = (t - (-15))*(66/255-216/255)/(-30+15) + 216/255;
                                //console.log(t+ " "+c2);
                                return Qt.rgba(0, c2, 1, 1);
                            }else if (t < -30){
                                c1 = (t - (-30))*(132/255-0)/(-30+15) + 0;
                                c2 = (t - (-30))*(0-66/255)/(-30+15) + 66/255;
                                return Qt.rgba(c1, c2, 1, 1);
                            }
                    	}

                		onCreationCompleted: {
                           if (Current.getdata(0, "temp") == "N/A"){
				                temp_text.text = ""
				                if (Current.getdata(0, "temp_high") != "N/A")
                                   temp_text.text = Current.getdata(0, "temp_high") + '°'
				                if ((Current.getdata(0, "temp_low") != "N/A") && (Current.getdata(0, "temp_high") != "N/A"))
				                if (Current.getdata(0, "temp_low") != "N/A")
                                   temp_text.text = temp_text.text + Current.getdata(0, "temp_low") + '°'
                                current_rect.background = Color.create(getColor(Current.getdata(0, "temp_high")));
                            }else{
                               temp_text.text = Current.getdata(0, "temp") + '°'
                               current_rect.background = Color.create(getColor(Current.getdata(0, "temp")));
				            }
                        }
                	}                	
                }
                Container{
                    preferredWidth: 768
                	layout: DockLayout {}
                	layoutProperties: AbsoluteLayoutProperties {
                	    positionY: 105.0
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
