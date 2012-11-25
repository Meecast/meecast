import bb.cascades 1.0


Page {
    id: fullweather 
    property int day: 10
    property bool current: false
    property string day_period: "day"
    property string day_period_name: ""
    property string image_source: ""
    property string description_text: ""

    property string map_pattern: "";  
    property string count_of_maps: "0";  

    property variant model_current: Current 
    property variant model_day:  Forecast_model
    property variant model_night:  (current) ? Current_night : Forecast_night_model
    property variant model_hours:  Forecast_hours_model
    //property variant description_text_alignment: Text.AlignHLeft;

    function updateperiod()
    {
        condition.clear()
        condition2.clear()
        console.log("Day period ", day_period);
	    if (day_period == "current"){
	     //   toolbarnow.checked = true
	     //   toolbarday.checked = false 
	     //   toolbarnight.checked = false
	     //   toolbarclock.checked = false
            day_rect.visible = true;
            current_rect.visible = true;
            hours_list.visible = false;
            flickable.contentHeight = day_rect.height + current_rect.height;

            day_period_name = Config.tr("Now")
            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_current.getdata(day, "pict")
            current_rect.color = getColor(model_current.getdata(day, "temp_high"));
            description_text = model_current.getdata(day, "description") ? model_current.getdata(day, "description") : ""
	   
            if ((model_current.getdata(day, "humidity")) != "N/A")
                condition.insert({"cond_name": Config.tr("Humidity:"),
                         "value": model_current.getdata(day, "humidity")+'%'});
            if ((model_current.getdata(day, "wind_direction")) != "")
                condition.insert({"cond_name": Config.tr("Wind direction:"),
                         "value": Config.tr(model_current.getdata(day, "wind_direction"))});
            if ((model_current.getdata(day, "pressure")) != "N/A")
                condition.insert({"cond_name": Config.tr("Pressure:"),
                         "value": model_current.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_current.getdata(day, "wind_speed")) != "N/A")
                condition.insert({"cond_name": Config.tr("Wind speed") + ":",
                         "value": model_current.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_current.getdata(day, "ppcp")) != "N/A")
                condition.insert({"cond_name": Config.tr("Ppcp:"),
                         "value": model_current.getdata(day, "ppcp") + '%'});
            if ((model_current.getdata(day, "wind_gust")) != "N/A")
                condition.insert({"cond_name": Config.tr("Wind gust:"),
                         "value": model_current.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_current.getdata(day, "flike")) != "N/A")
                condition.insert({"cond_name": Config.tr("Flike:"),
                         "value": model_current.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_current.getdata(day, "map_pattern")) != ""){
                map_pattern = model_current.getdata(day, "map_pattern")
                map_text.visible = true;
                count_of_maps = model_current.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;

            if ((model_current.getdata(day, "temp")) != "N/A")
                temperature.text =  model_current.getdata(day, "temp") + '°'
     	    else{
                if ((model_current.getdata(day, "temp_high")) != "N/A")
               	    temperature.text =  model_current.getdata(day, "temp_high") + '°'
            }
            if ((model_current.getdata(day, "visible")) != "N/A")
                condition.insert({"cond_name": Config.tr("Visible:"),
                         "value": model_current.getdata(day, "visible") + ' ' + Config.tr(Config.visibleunit)});
            if ((model_current.getdata(day, "uv_index")) != "")
                condition.insert({"cond_name": Config.tr("UV index:"),
                         "value": model_current.getdata(day, "uv_index")});
	}

        if (day_period == "day"){
//	    toolbarnow.checked = false 
//	    toolbarday.checked = true
//	    toolbarnight.checked = false
//	    toolbarclock.checked = false
        //    day_rect.visible = true;
        //    current_rect.visible = true;
        //    hours_list.visible = false;
        //    flickable.contentHeight = day_rect.height + current_rect.height;

            day_period_name = Config.tr("Day")
            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_day.getdata(day, "pict")
            current_rect_back.background = Color.create(main.getColor(model_day.getdata(0, "temp_high")));
            description_text = model_day.getdata(day, "description") ? model_day.getdata(day, "description") : ""
	   
            if ((model_day.getdata(day, "humidity")) != "N/A")
                condition.insert({"cond_name": Config.tr("Humidity:"),
                         "value": model_day.getdata(day, "humidity")+'%'});
            if ((model_day.getdata(day, "wind_direction")) != "")
                condition.insert({"cond_name": Config.tr("Wind direction:"),
                         "value": Config.tr(model_day.getdata(day, "wind_direction"))});
            if ((model_day.getdata(day, "pressure")) != "N/A")
                condition.insert({"cond_name": Config.tr("Pressure:"),
                         "value": model_day.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_day.getdata(day, "wind_speed")) != "N/A")
                condition.insert({"cond_name": Config.tr("Wind speed") + ":",
                         "value": model_day.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_day.getdata(day, "ppcp")) != "N/A")
                condition.insert({"cond_name": Config.tr("Ppcp:"),
                         "value": model_day.getdata(day, "ppcp") + '%'});
            if ((model_day.getdata(day, "wind_gust")) != "N/A")
                condition.insert({"cond_name": Config.tr("Wind gust:"),
                         "value": model_day.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_day.getdata(day, "flike")) != "N/A")
                condition.insert({"cond_name": Config.tr("Flike:"),
                         "value": model_day.getdata(day, "flike") + '°' + Config.temperatureunit});

            if ((model_day.getdata(day, "temp")) != "N/A")
                temperature.text =  model_day.getdata(day, "temp") + '°'
	        else{
                if ((model_day.getdata(day, "temp_high")) != "N/A")
               	    temperature.text =  model_day.getdata(day, "temp_high") + '°'
            }
            if ((model_day.getdata(day, "map_pattern")) != ""){
                map_pattern = model_day.getdata(day, "map_pattern")
                //map_text.visible = true;
                count_of_maps = model_day.getdata(day, "count_of_maps")
            }else{
            //    map_text.visible = false;
            }
	}
	if (day_period == "night"){
            day_period_name = Config.tr("Night");
            toolbarnow.checked = false;
            toolbarnight.checked = true;
            toolbarday.checked = false;
            toolbarclock.checked = false;
            day_rect.visible = true;
            current_rect.visible = true;
            hours_list.visible = false;
            flickable.contentHeight = day_rect.height + current_rect.height;

            image_source = Config.iconspath + "/" + Config.iconset + "/" + model_night.getdata(day, "pict");
            current_rect.color = getColor(model_day.getdata(day, "temp_low"));
            description_text = model_night.getdata(day, "description") ? model_night.getdata(day, "description") : ""
            if ((model_night.getdata(day, "humidity")) != "N/A")
                condition.insert({"cond_name": Config.tr("Humidity:"),
                         "value": model_night.getdata(day, "humidity")+'%'});
            if ((model_night.getdata(day, "wind_direction")) != "")
                condition.insert({"cond_name": Config.tr("Wind direction:"),
                         "value": Config.tr(model_night.getdata(day, "wind_direction"))});
            if ((model_night.getdata(day, "pressure")) != "N/A")
                condition.insert({"cond_name": Config.tr("Pressure:"),
                         "value": model_night.getdata(day, "pressure") + ' ' + Config.tr(Config.pressureunit)});
            if ((model_night.getdata(day, "wind_speed")) != "N/A")
                condition.insert({"cond_name": Config.tr("Wind speed") + ":",
                         "value": model_night.getdata(day, "wind_speed") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_night.getdata(day, "ppcp")) != "N/A")
                condition.insert({"cond_name": Config.tr("Ppcp:"),
                         "value": model_night.getdata(day, "ppcp") + '%'});
            if ((model_night.getdata(day, "wind_gust")) != "N/A")
                condition.insert({"cond_name": Config.tr("Wind gust:"),
                         "value": model_night.getdata(day, "wind_gust") + ' ' + Config.tr(Config.windspeedunit)});
            if ((model_night.getdata(day, "flike")) != "N/A")
                condition.insert({"cond_name": Config.tr("Flike:"),
                         "value": model_night.getdata(day, "flike") + '°' + Config.temperatureunit});
            if ((model_night.getdata(day, "temp")) != "N/A")
                temperature.text =  model_night.getdata(day, "temp") + '°'
	        else{
                if ((model_night.getdata(day, "temp_low")) != "N/A")
                    temperature.text =  model_night.getdata(day, "temp_low") + '°'
            }
            if ((model_night.getdata(day, "map_pattern")) != ""){
                map_pattern = model_night.getdata(day, "map_pattern")
                map_text.visible = true;
                count_of_maps = model_night.getdata(day, "count_of_maps")
            }else
                map_text.visible = false;

	}
	if (day_period == "hours"){
            day_period_name = Config.tr("Hours");
	        toolbarnow.checked = false;
            toolbarnight.checked = false;
            toolbarday.checked = false;
            toolbarclock.checked = true;
            day_rect.visible = true;
            current_rect.visible = false;
            hours_list.visible = true;
            flickable.contentHeight = hours_list.height + day_rect.height;

	}
        if ((model_day.getdata(day, "sunrise")) != "N/A")
            condition2.insert({"cond_name": Config.tr("Sunrise:"),
                         "value": model_day.getdata(day, "sunrise")});
        if ((model_day.getdata(day, "sunset")) != "N/A")
            condition2.insert({"cond_name": Config.tr("Sunset:"),
                         "value": model_day.getdata(day, "sunset")});
        if ((model_day.getdata(day, "daylength")) != "N/A")
            condition2.insert({"cond_name": Config.tr("Day length:"),
                         "value": model_day.getdata(day, "daylength")});
        if ((model_day.getdata(day, "lastupdate")) != "N/A")
            condition2.insert({"cond_name": Config.tr("Last update:"),
                         "value": model_day.getdata(day, "lastupdate")});
	
//	if (description_text.length > 35){
//	    description_text_alignment = Text.AlignHLeft
//	}else{
//	    description_text_alignment = Text.AlignHCenter
//	}

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
        console.log("updatestationname() ", Config.stationname );
        console.log(" pATH !!!! ", Config.imagespath);
        main.updatemodels();
        stationname.text = Config.stationname == "Unknown" ? "MeeCast" : Config.stationname
        left_arrow.visible = Config.prevstationname == "" ? false : true;
        right_arrow.visible = Config.nextstationname == "" ? false : true;
        sourceicon.visible = false;
        sourceicon.imageSource = Config.imagespath + "/" + Config.source + ".png";
        sourceicon.visible = true;
    }


    function onConfigChanged() {
        console.log("end update station name = "+Config.stationname);
        startview.visible = Config.stationname == "Unknown" ? true : false;
//            mainview.visible = Config.stationname == "Unknown" ? false : true;
        main.updatestationname();
        //isUpdate = false;
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
        //dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
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
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 560
            }
            background: Color.Black
            preferredWidth: 768
            preferredHeight: 1000
                   onTouch: {
                                    background = Color.Green;
                                    console.log("dfffffffffffffffffffff");
                                    if (event.isDown()) {
                                        var newPage = fullpageDefinition.createObject();
                                        rootWindow.push(newPage);
                                    }
                                }
            
        }

        Container{
            background: Color.Black
            preferredWidth: 768
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 95
            }
            
            id: current_rect
            visible: Current.rowCount() == 0 ? false : true
            layout: AbsoluteLayout {
            }
            Container{
                id: current_rect_back
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
                visible: true
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
                        text: day_period_name 
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
                        id: temperature 
                //      text: Current.getdata(0, "temp") + '°';
                        horizontalAlignment: HorizontalAlignment.Center
                        verticalAlignment: VerticalAlignment.Center
                        textStyle.textAlign: TextAlign.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                            color: Color.White
                        }
                        onCreationCompleted: {
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
                    text: description_text 
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle {
                       base: SystemDefaults.TextStyles.BodyText
                       color: Color.White
                    }
                }
            }
                  
            }
            Container{
                preferredWidth: 768
                layout: DockLayout {}
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                }
                background: Color.Black
                ImageButton {                 
                   id: left_arrow
                   visible: day > 0 ? true : false;
                   horizontalAlignment: HorizontalAlignment.Left
                   verticalAlignment: VerticalAlignment.Center
                   preferredWidth: 62*1.6
                   preferredHeight: 62*1.6
                   defaultImageSource: "asset:///share/images/arrow_left.png"
                   onClicked: {
                        if (day > 0){
                            console.log("prev day");
                            day--;
                            fullweather.updateperiod();
                            dayname.text = (fullweather.current && fullweather.day == 0) ? Config.tr("Today") : model_day.getdata(day, "date");
                        }

                   }
                }
                Container{
                    layout: DockLayout {}
                    preferredWidth: 600 
                    horizontalAlignment: HorizontalAlignment.Center 
                    Label {                 
                        id: dayname
                        horizontalAlignment: HorizontalAlignment.Center
                        textStyle {
                            base: SystemDefaults.TextStyles.BigText
                            color: Color.White
                        }
                        onCreationCompleted: {
                            fullweather.current = main.current
                            fullweather.day = main.day
                            fullweather.day_period = main.day_period
                            fullweather.updateperiod();
                            text = (fullweather.current && fullweather.day == 0) ? Config.tr("Today") : model_day.getdata(day, "date");
                        }
                    }
            }
            ImageButton {                 
               id: right_arrow
               visible: day < (model_day.rowCount()-1) ? true : false;
               verticalAlignment: VerticalAlignment.Center
               horizontalAlignment: HorizontalAlignment.Right
               defaultImageSource: "asset:///share/images/arrow_right.png"
               preferredWidth: 62*1.6
               preferredHeight: 62*1.6
               onClicked: {
                    if (day < model_day.rowCount()-1){ 
                        console.log("next day");
                        day_period="day";
                        day++;
                        fullweather.updateperiod();
                        dayname.text = (fullweather.current && fullweather.day == 0) ? Config.tr("Today") : model_day.getdata(day, "date");
                    }
               }
            }
        }
        ImageView {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 768/2 - 64 *1.6
                positionY: 80
            }
            preferredWidth: 128*1.6
            preferredHeight: 128*1.6
            imageSource: image_source 
            horizontalAlignment: HorizontalAlignment.Center                
        } 
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 370
            }
            preferredWidth: 768
            preferredHeight: 800.0
            ListView {
                id: forrecasts_grid_list
                layout: GridListLayout {
                    columnCount : 2
                    cellAspectRatio: 2.5
                }
                dataModel: GroupDataModel {
                    id: condition
                    grouping: ItemGrouping.None
                    sortedAscending: false
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container{
                            layout: StackLayout {
                            }
                            Label {
                                text: ListItemData.cond_name
                                textStyle {    
                                    base: SystemDefaults.TextStyles.BodyText
                                    color: Color.Gray
                                }
                            }
                            Label {
                                text: ListItemData.value
                                textStyle {
                                    base: SystemDefaults.TextStyles.BodyText
                                    color: Color.White
                                }
                            }
                        } 
                    }
                ]
            }
            Divider {
                horizontalAlignment: HorizontalAlignment.Fill
            }
            ListView {
                id: forrecasts_grid_list_additional
                layout: GridListLayout {
                    columnCount : 2
                    cellAspectRatio: 2.5
                }
                dataModel: GroupDataModel {
                    id: condition2
                    grouping: ItemGrouping.None
                }
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        Container{
                            layout: StackLayout {
                            }
                            Label {
                                text: ListItemData.cond_name
                                textStyle {    
                                    base: SystemDefaults.TextStyles.BodyText
                                    color: Color.Gray
                                }
                            }
                            Label {
                                text: ListItemData.value
                                textStyle {
                                    base: SystemDefaults.TextStyles.BodyText
                                    color: Color.White
                                }
                            }
                        } 
                    }
                ]
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
            id: sourcepageDefinition
            source: "SourcePage.qml"
        },
        ComponentDefinition {
            id: fullpageDefinition
            source: "FullWeatherPage.qml"
        },
        ComponentDefinition {
            id: sboutpageDefinition
            source: "AboutPage.qml"
        }
    ]

} // end of Page
