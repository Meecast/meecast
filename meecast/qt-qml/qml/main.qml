import bb.cascades 1.0


NavigationPane {
    
    id: rootWindow
    // Create the initial screen
    Page {
        id: main
    	property int screen_width : 1280
        property int screen_height : 768


		function update_list(){
    		for (var a = 0; a < Forecast_model.rowCount(); a++) {
				forrecasts_list.dataModel.insert(
					{
					 "fulldate" : Forecast_model.getdata(a, "fulldate") + '.',
					 "shortdate" : Forecast_model.getdata(a, "shortdate"),
					 "pict" : Config.iconspath + "/" + Config.iconset + "/" + Forecast_model.getdata(a, "pict"),
					 "temp_high" : Forecast_model.getdata(a, "temp_high"),
					 "temp_low" : Forecast_model.getdata(a, "temp_low"),
					 "bg_color" :  (a % 2 != 0) ?  "#0f0f0f" : "#000000",
					 "hi_temp_color" :  main.getColor(Forecast_model.getdata(a, "temp_high"))
					}
				)
			}
		}
		function update_current_data(){
			if (Current.getdata(0, "temp") == "N/A"){
				current_temp_text.text = ""
				if (Current.getdata(0, "temp_high") != "N/A")
				   current_temp_text.text = Current.getdata(0, "temp_high") + '°'
				if ((Current.getdata(0, "temp_low") != "N/A") && (Current.getdata(0, "temp_high") != "N/A"))
				if (Current.getdata(0, "temp_low") != "N/A")
				   current_temp_text.text = current_temp_text.text + ' / '+ Current.getdata(0, "temp_low") + '°'
				current_rect.background = Color.create(main.getColor(Current.getdata(0, "temp_high")));
			}else{
			   current_temp_text.text = Current.getdata(0, "temp") + '°'
			   current_rect.background = Color.create(main.getColor(Current.getdata(0, "temp")));
			}


		}

		function getColor(t) {
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
 
        function updatestationname(){
			main.updatemodels();
            stationname.text = Config.stationname == "Unknown" ? Config.stationname : "MeeCast"
//			stationname.text = Config.stationname;
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
			main.update_list();
			main.update_current_data();
			//list.height = 80 * Forecast_model.rowCount();
			console.debug ("Forecast_model.rowCount()", Forecast_model.rowCount(), Current.rowCount());
			dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
			current_rect.visible = Current.rowCount() == 0 ? false : true;
			//list.visible = (Forecast_model.rowCount() == 0) ? false : true;
        }

   
        content: Container {
            background: Color.Black

            onCreationCompleted: {
                 Config.configChanged.connect (main.onConfigChanged);
            }
            layout: AbsoluteLayout {}
            Container{
            	background: Color.Black
            	preferredWidth: 768
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 95
                }
                layout: AbsoluteLayout {
                }
				Container {
					id: current_rect
  				    visible: Current.rowCount() == 0 ? false : true
				  	preferredWidth: 768
					Container{
				    	preferredWidth: 768
						preferredHeight: 438
						layoutProperties: AbsoluteLayoutProperties {
							positionY: 30
						}
						background: Color.Red
					}
					ImageView {
						imageSource: "asset:///share/images/mask_background_main.png"
						preferredWidth: 768
						preferredHeight: 438
						layoutProperties: AbsoluteLayoutProperties {
							   positionY: 30
						}
					}
					Container{
						preferredWidth: 768
						layout: DockLayout {}
						layoutProperties: AbsoluteLayoutProperties {
							positionY: 50.0
						}
						Container{
							horizontalAlignment: HorizontalAlignment.Left
							preferredWidth: 768/2 - 128*1.6/2 
							Label {
								text: Current.getdata(0, "current") == true ? Config.tr("Now") : Config.tr("Today")
								horizontalAlignment: HorizontalAlignment.Center
								verticalAlignment: VerticalAlignment.Center
								textStyle.textAlign: TextAlign.Center
								textStyle {
									base: SystemDefaults.TextStyles.BigText
									color: Color.White
								}
							}
						}
						Container{
							id: current_temperature
							horizontalAlignment: HorizontalAlignment.Right
							preferredWidth: 768/2 - 128*1.6/2                	
							Label {                 
								id: current_temp_text
						//		text: Current.getdata(0, "temp") + '°';
								horizontalAlignment: HorizontalAlignment.Center
								verticalAlignment: VerticalAlignment.Center
								textStyle.textAlign: TextAlign.Center
								textStyle {
									base: SystemDefaults.TextStyles.BigText
									color: Color.White
								}
								onCreationCompleted: {
									main.update_current_data();
								}
							}    
						}            	
					}
					Container{
						id: title
						preferredWidth: 768
						layout: DockLayout {}
						layoutProperties: AbsoluteLayoutProperties {
							positionY: 165.0
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
					Container{
						id: humidity_and_wind_direction
						preferredWidth: 768
						layout: StackLayout {
							orientation: LayoutOrientation.LeftToRight
						}
						layoutProperties: AbsoluteLayoutProperties {
							positionY: 290.0
						}
						Container{
							id: humidity
							horizontalAlignment: HorizontalAlignment.Left
							preferredWidth: 768/2
							layout: StackLayout {
								orientation: LayoutOrientation.LeftToRight
							}
							ImageView {
								imageSource: "asset:///share/images/humidity.png"
								preferredWidth: 30*1.6
								preferredHeight: 30*1.6
							}
							Label {
								text: Current.getdata(0, "humidity") + " %";
								verticalAlignment: VerticalAlignment.Center
								//horizontalAlignment: HorizontalAlignment.Left
								textStyle {
									base: SystemDefaults.TextStyles.BodyText
									color: Color.White
								}
							}
						}
						Container{
							id: wind_direction
							function getAngle(s)
							{
								var a;
								switch (s){
								case 'S':
									return 0;
								case 'SSW':
									return 22.5;
								case 'SW':
									return 45;
								case 'WSW':
									return (45+22.5);
								case 'W':
									return 90;
								case 'WNW':
									return (90+22.5);
								case 'NW':
									return (90+45);
								case 'NNW':
									return (180-22.5);
								case 'N':
									return 180;
								case 'NNE':
									return (180+22.5);
								case 'NE':
									return (180+45);
								case 'ENE':
									return (270-22.5);
								case 'E':
									return 270;
								case 'ESE':
									return (270+22.5);
								case 'SE':
									return (270+45);
								case 'SSE':
									return (360-22.5);

								}

							}

							horizontalAlignment: HorizontalAlignment.Left
							layout: StackLayout {
								orientation: LayoutOrientation.LeftToRight
							}
							Container{
								layout: AbsoluteLayout {
								}
								ImageView {
									imageSource: "asset:///share/images/wind_direction_background.png"
									preferredWidth: 30*1.6
									preferredHeight: 30*1.6
								}
								ImageView {
									imageSource: "asset:///share/images//wind_direction_arrow.png"
									preferredWidth: 30*1.6
									preferredHeight: 30*1.6
									rotationZ: wind_direction.getAngle(Current.getdata(0, "wind_direction"))
								}
							}
							Label {
								text: Current.getdata(0, "wind_direction");
								textStyle {
									base: SystemDefaults.TextStyles.BodyText
									color: Color.White
								}
							}
						}        
					}
					Container{
						id: pressure_and_wind_speed
						preferredWidth: 768
						layout: StackLayout {
							orientation: LayoutOrientation.LeftToRight
						}
						layoutProperties: AbsoluteLayoutProperties {
							positionY: 390.0
						}
						Container{
							id: pressure
							horizontalAlignment: HorizontalAlignment.Left
							preferredWidth: 768/2
							layout: StackLayout {
								orientation: LayoutOrientation.LeftToRight
							}
							ImageView {
								imageSource: "asset:///share/images/pressure.png"
								preferredWidth: 30*1.6 
								preferredHeight: 30*1.6
							}
							Label {
								text: Current.getdata(0, "pressure") + ' ' + Config.tr(Config.pressureunit) ;
								//horizontalAlignment: HorizontalAlignment.Left
								textStyle {
									base: SystemDefaults.TextStyles.BodyText
									color: Color.White
								}
							}
						}
						Container{
							id: wind_speed
							horizontalAlignment: HorizontalAlignment.Left
							layout: StackLayout {
								orientation: LayoutOrientation.LeftToRight
							}
							ImageView {
								imageSource: "asset:///share/images/wind_speed.png"
								preferredWidth: 30*1.6
								preferredHeight: 30*1.6
							}
							Label {
								text: Current.getdata(0, "wind_speed") + ' ' + Config.tr(Config.windspeedunit);
								textStyle {
									base: SystemDefaults.TextStyles.BodyText
									color: Color.White
								}
							}
						}        
					}			        
				} 
		    }
        	Container{
                layoutProperties: AbsoluteLayoutProperties {
                	positionX: 0
                    positionY: 560
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
					id: stationname
			        horizontalAlignment: HorizontalAlignment.Center 
				    Label {                 
				        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
					        base: SystemDefaults.TextStyles.BigText
					        color: Color.White
				        }
                    	onCreationCompleted: {
				        	text = Config.stationname == "Unknown" ? "MeeCast" : Config.stationname 
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
            ImageView {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 768/2 - 64 *1.6
                    positionY: 60
                }
                preferredWidth: 128*1.6
                preferredHeight: 128*1.6
                imageSource: Config.iconspath + "/" + Config.iconset + "/" + Current.getdata(0, "pict")
                horizontalAlignment: HorizontalAlignment.Center                
            } 
            Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 500
                }
                preferredWidth: 768
                ListView {
                    id: forrecasts_list
                    dataModel: GroupDataModel {
                    }
                    listItemComponents: [
                        ListItemComponent {
                             type: "header"
                             Label { text: "" }
                        },
                        ListItemComponent {
                            type: "item"
                            Container{
                                layout: DockLayout {
                                }
                                background: Color.create(ListItemData.bg_color)
                                preferredWidth: 768
                                Container{
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    preferredWidth: 768/2
                                    verticalAlignment: VerticalAlignment.Center
                                    horizontalAlignment: HorizontalAlignment.Left
                                    Container{
                                        preferredWidth: 20
                                        maxWidth: 20
                                    }
                                    Label {
                                        verticalAlignment: VerticalAlignment.Center
                                        text: ListItemData.fulldate
                                        textStyle {    
                                            base: SystemDefaults.TextStyles.BodyText
                                            color: Color.Gray
                                        }
                                    }
                                    Label {
                                        text: ListItemData.shortdate
                                        textStyle {
                                            base: SystemDefaults.TextStyles.BodyText
                                            color: Color.White
                                        }
                                    }
                                }
                                ImageView {
                                     imageSource: ListItemData.pict
                                     horizontalAlignment: HorizontalAlignment.Center                
                                }
                                Container{
                                    layout: StackLayout {
                                        orientation: LayoutOrientation.LeftToRight
                                    }
                                    
                                    horizontalAlignment: HorizontalAlignment.Right
                                    verticalAlignment: VerticalAlignment.Center
                                    Label {
                                        text: ListItemData.temp_high + '°'
                                        horizontalAlignment: HorizontalAlignment.Right
                                        preferredWidth: 100
                                        textStyle.textAlign: TextAlign.Right
                                        textStyle {
                                            base: SystemDefaults.TextStyles.BodyText
                                            color: Color.create(ListItemData.hi_temp_color)
                                        }
                                    }
                                    Label {
                                        text: ListItemData.temp_low + '°'
                                        horizontalAlignment: HorizontalAlignment.Right
                                        preferredWidth: 100
                                        textStyle.textAlign: TextAlign.Right
                                        textStyle {
                                            base: SystemDefaults.TextStyles.BodyText
                                            color: Color.create("#889397")
                                        }
                                    }
                                    Container{
                                        preferredWidth: 20
                                        maxWidth: 20
                                    }
                                }                                                                                                   
                            } 
                        }
                    ]
                    onCreationCompleted: {
                        main.update_list();
                    }
                }
			}
			Container{
			    layoutProperties: AbsoluteLayoutProperties {
			        positionX: 0 
			        positionY: 1140
			    }
			    preferredHeight: 2
			    preferredWidth: 768
			    background: Color.Gray    
			}
			Container{
				layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 1142
                }
                preferredWidth: 768 
                preferredHeight: 138 
                background: Color.Black
                Container{    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Container{
                        preferredWidth: 20
                    }
                    ImageButton {
                        id: refreshicon
                        verticalAlignment: VerticalAlignment.Center
                        defaultImageSource: "asset:///button_icons/icon_refresh.png"
                        onClicked: {
                            Config.updatestations()
                        }
                    }
                    ImageView {
                        id: sourceicon
                        verticalAlignment: VerticalAlignment.Center
                        imageSource: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"                 
                        preferredWidth: 140*1.6
                        preferredHeight: 31*1.6
                        leftMargin: 220.0
                    }
                    
                    ImageButton {
                        id: settingsicon
                        verticalAlignment: VerticalAlignment.Center     
                        defaultImageSource: "asset:///button_icons/icon_settings.png"
                        leftMargin: 220.0
                        horizontalAlignment: HorizontalAlignment.Right
                        onClicked: {
                            var newPage = settingspageDefinition.createObject();
                            rootWindow.push(newPage);
                        }
                    }
                    Container{
                        preferredWidth: 20
                        preferredHeight: 138 
                    }
                }
			}
        }
         
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
