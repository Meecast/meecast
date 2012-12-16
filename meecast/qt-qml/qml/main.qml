import bb.cascades 1.0


NavigationPane {
    
    id: rootWindow
    property color current_rect_back_background :  "Red" 
    property bool feel_like_temperature_visible : false
    property string feels_like_temp_text : ""
    property string main_icon_imageSource : "" 
    property string title_text_text : "1111"
    property string current_temp_text: ""
    property string humidity_text: "" 
    property string pressure_text: ""
    property string wind_speed_text: ""
    property string wind_direction_text: ""    
    property bool  left_arrow : false
    property bool  right_arrow : false

    // Create the initial screen
    Page {
        id: main
        property int screen_width : 1280
        property int screen_height : 768
        property int day: 0
        property bool current: false
        property string day_period: "day"
        property bool isUpdate: false

        function update_list(){
            forrecasts_list.dataModel.clear();
            for (var a = 0; a < Forecast_model.rowCount() + 1; a++) {
                if (a == 0){
                    forrecasts_list.dataModel.insert(
                        {
                         "fulldate" : Forecast_model.getdata(a, "fulldate"),
                         "shortdate" : Forecast_model.getdata(a, "shortdate"),
                         "temp_high" : Forecast_model.getdata(a, "temp_high"),
                         "temp_low" : Forecast_model.getdata(a, "temp_low"),
                         "bg_color" :  (a % 2 != 0) ? "#000000" : "#0f0f0f",
                         "hi_temp_color" :  main.getColor(Forecast_model.getdata(a, "temp_high")),
                         "wind_speed" : Forecast_model.getdata(a, "wind_speed"), 
                         "number" : a,
                         "bg_color" : current_rect_back_background,
                         "stationname" : Config.stationname == "Unknown" ? "MeeCast" : Config.stationname,
                         "feel_like_temperature_visible" : feel_like_temperature_visible,
                         "feels_like_temp_text" : feels_like_temp_text, 
                         "pict" : main_icon_imageSource,
                         "description_text" : title_text_text,
                         "current_temp_text" : current_temp_text,
                         "wind_direction" : wind_direction_text, 
                         "wind_speed" : wind_speed_text, 
                         "humidity" : humidity_text, 
                         "pressure" : pressure_text, 
                         "left_arrow" : left_arrow,
                         "right_arrow" : right_arrow,
                        }
                    )
                }else{
                    forrecasts_list.dataModel.insert(
                        {
                         "fulldate" : Forecast_model.getdata(a-1, "fulldate"),
                         "shortdate" : Forecast_model.getdata(a-1 , "shortdate"),
                         "pict" : Config.iconspath + "/" + Config.iconset + "/" + Forecast_model.getdata(a-1, "pict"),
                         "temp_high" : Forecast_model.getdata(a-1, "temp_high"),
                         "temp_low" : Forecast_model.getdata(a-1, "temp_low"),
                         "bg_color" :  ((a-1) % 2 != 0) ? "#000000" : "#0f0f0f",
                         "hi_temp_color" :  main.getColor(Forecast_model.getdata(a-1, "temp_high")),
                         "wind_speed" : Forecast_model.getdata(a-1, "wind_speed"), 
                         "number" : a,
                         "feel_like_temperature_visible" : feel_like_temperature_visible,  
                         "description_text" : Forecast_model.getdata(a-1, "description"),
                         "wind_direction" : Forecast_model.getdata(a-1, "wind_direction"), 
                         "current_temp_text" : current_temp_text,
                         "humidity" : Forecast_model.getdata(a-1, "humidity"), 
                         "pressure" : Forecast_model.getdata(a-1, "pressure"), 
                         "left_arrow" : left_arrow,
                         "right_arrow" : right_arrow,
                        }
                    )
                }
            }
        }

        function update_current_data(){
            console.log("update_current_data!!!!!!!!!!!!!");
            left_arrow = Config.prevstationname == "" ? false : true;
            right_arrow = Config.nextstationname == "" ? false : true;

            if (Current.getdata(0, "temp") == "N/A"){
                current_temp_text = ""
                if (Current.getdata(0, "temp_high") != "N/A")
                   current_temp_text = Current.getdata(0, "temp_high") + '°'
                if ((Current.getdata(0, "temp_low") != "N/A") && (Current.getdata(0, "temp_high") != "N/A"))
                if (Current.getdata(0, "temp_low") != "N/A")
                   current_temp_text = current_temp_text + ' / '+ Current.getdata(0, "temp_low") + '°'
                current_rect_back_background = main.getColor(Current.getdata(0, "temp_high"));
            }else{
               current_temp_text = Current.getdata(0, "temp") + '°'
               console.log(Current.getdata(0, "temp"));
               if (Current.getdata(0, "temp") > -273){
                   current_rect_back_background = main.getColor(Current.getdata(0, "temp"));
               }
               
            }
            if (Current.getdata(0, "flike") == "N/A"){
                feel_like_temperature_visible = false;
            }else{
                feel_like_temperature_visible = true;
                feels_like_temp_text = Current.getdata(0, "flike") + '°'
            }
            main_icon_imageSource = Config.iconspath + "/" + Config.iconset + "/" + Current.getdata(0, "pict")
            title_text_text = Config.tr(Current.getdata(0, "description"));
            humidity_text = Current.getdata(0, "humidity") + " %";
            pressure_text = Current.getdata(0, "pressure") + ' ' + Config.tr(Config.pressureunit) ;

            wind_speed_text = Current.getdata(0, "wind_speed") + ' ' + Config.tr(Config.windspeedunit);
            //wind_direction_arrow.rotationZ =  wind_direction.getAngle(Current.getdata(0, "wind_direction"));
            wind_direction_text = Config.tr(Current.getdata(0, "wind_direction"));


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
        function getAngle(s){
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

        function updatestationname(){
            console.log("updatestationname() ", Config.stationname );
            console.log(" pATH !!!! ", Config.imagespath);
            main.updatemodels();
            //stationname_text = Config.stationname == "Unknown" ? "MeeCast" : Config.stationname

            //left_arrow = Config.prevstationname == "" ? false : true;
            //right_arrow = Config.nextstationname == "" ? false : true;
            sourceicon.visible = false;
            sourceicon.imageSource = Config.imagespath + "/" + Config.source + ".png";
            sourceicon.visible = true;
        }


        function onConfigChanged() {
            console.log("end update station name = "+Config.stationname);
            startview.visible = Config.stationname == "Unknown" ? true : false;
//            mainview.visible = Config.stationname == "Unknown" ? false : true;
            main.updatestationname();
            main.isUpdate = false;
        }

        function updatemodels()
        {
            Current.reload_data(Config.filename);
            Current.update_model(0);
            Current_night.update_model(1);
            main.update_current_data();
            Forecast_model.update_model(2);
            Forecast_night_model.update_model(3);
            Forecast_hours_model.update_model(4);
            main.update_list();
            //list.height = 80 * Forecast_model.rowCount();
            //dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
            //current_rect.visible = Current.rowCount() == 0 ? false : true;
            //list.visible = (Forecast_model.rowCount() == 0) ? false : true;
        }

   
        content:
            Container {
            layout: AbsoluteLayout {}
            Container {
               // preferredHeight: 1142.0
                //preferredHeight: 1442.0
                //scrollViewProperties.scrollMode: ScrollMode.Vertical
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: 0
                }  
                
            
                Container {
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
                    }

                    Container{
                        id: startview
                        visible : Config.stationname == "Unknown" ? true : false;
                        preferredWidth: 768
                        background: Color.Black
                        layout: AbsoluteLayout {
                        }
                        Container{
                            preferredWidth: 768
                            preferredHeight: 438
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 30
                            }
                            background: Color.White

                        }
                        ImageView {
                            imageSource: "asset:///share/images/mask_background.png"
                            preferredWidth: 768
                            preferredHeight: 438
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 30
                            }
                        }
                        Label {
                            text: Config.tr("No locations are set up yet.") 
                            horizontalAlignment: HorizontalAlignment.Center
                            preferredWidth: 768 
                            multiline: true
                            textStyle.textAlign: TextAlign.Center
                            textStyle {
                                    base: SystemDefaults.TextStyles.BigText
                                    color: Color.create("#999999")
                            }
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 400 
                            }
                        }
                    }
                    
                Container{
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 0
                        positionY: 0
                    }
                    preferredWidth: 768
                    preferredHeight: 1140 
                    ListView {
                        id: forrecasts_list
                        dataModel: GroupDataModel {
                            id: groupDataModel
                            sortingKeys: ["number"]
                            grouping: ItemGrouping.None
                        }
                        onCreationCompleted: {
                            main.update_current_data();
                            Qt.Config = Config
                            Qt.Current = Current
                            Qt.main = main
                            Qt.left_arrow = left_arrow
                            Qt.right_arrow = right_arrow

                            main.update_list();
                        }

                        listItemComponents: [
                            ListItemComponent {
                                type: "item"
                                Container{
                                    Container{
                                    layout: AbsoluteLayout {}
                                    visible: ListItemData.number == 0 ? true : false;

                       Container{
                        background: Color.Black
                        preferredWidth: 768
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 0
                            positionY: 95
                        }
                      //  onTouch: {
                      //      if (event.isDown()) {
                      //          main.day = 0;
                      //          main.day_period = "day";
                      //          main.current = true;
                      //          var newPage = fullpageDefinition.createObject();
                      //          rootWindow.push(newPage);
                      //     }
                      //  }
                        
                        id: current_rect
                        //visible: Current.rowCount() == 0 ? false : true
                        layout: AbsoluteLayout {
                        }
                        Container{
                            id: current_rect_back
                            preferredWidth: 768
                            preferredHeight: 438
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 30
                            }
                            background: Color.create(ListItemData.bg_color)
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
                                id: feel_like_temperature
                                visible: ListItemData.feel_like_temperature_visible 
                                horizontalAlignment: HorizontalAlignment.Right
                                preferredWidth: 768/2 - 128/2                   

                                Container{
                                    layout: AbsoluteLayout {} 
                                    Label { 
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 0.0
                                        }
                                        id: feels_like_temp_description
                                        preferredWidth: 768/2 - 128/2 
                                        text: Qt.Config.tr("Feels like")
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle.textAlign: TextAlign.Center
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SmallText
                                            color: Color.Gray
                                        }
                                    }
                                    Label {
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 20.0
                                        }
                         
                                        preferredWidth: 768/2 - 128/2 
                                        id: feels_like_temp_text_id
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle.textAlign: TextAlign.Center
                                        text: ListItemData.feels_like_temp_text 
                                        textStyle {
                                            base: SystemDefaults.TextStyles.BigText
                                            color: Color.White
                                        }
                                    }    
                                }
                            }               
                            Container{
                                id: current_temperature
                                horizontalAlignment: HorizontalAlignment.Left
                                preferredWidth: 768/2 - 128/2 
                                Container{
                                    layout: AbsoluteLayout {}
                                    Label { 
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 0.0
                                        }
                                        id: current_temp_description
                                        preferredWidth: 768/2 - 128/2 
                                        text: Qt.Config.tr("Temperature")
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle.textAlign: TextAlign.Center
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SmallText
                                            color: Color.Gray
                                        }
                                    }

                                    Label {
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 20.0
                                        }

                                        id: current_temp_text_id
                                        text: ListItemData.current_temp_text
                                        preferredWidth: 768/2 - 128/2 
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle.textAlign: TextAlign.Center
                                        textStyle {
                                            base: SystemDefaults.TextStyles.BigText
                                            color: Color.White
                                        }
                                        onCreationCompleted: {
                                          //  main.update_current_data();
                                        }
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
                                id: title_text
                                text: ListItemData.description_text 
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
                                positionY: 260.0
                            }
                            Container{
                                id: humidity
                                horizontalAlignment: HorizontalAlignment.Left
                                preferredWidth: 768/2
                                layout: AbsoluteLayout {
                                }
                                Label { 
                                    layoutProperties: AbsoluteLayoutProperties {
                                        positionY: 0.0
                                        positionX: 20.0
                                    }
                                    id: humidity_description
                                    preferredWidth: 768/2 - 128/2 - 20
                                    text: Qt.Config.tr("Humidity")
                                    horizontalAlignment: HorizontalAlignment.Center
                                    verticalAlignment: VerticalAlignment.Center
                                    textStyle.textAlign: TextAlign.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Color.Gray
                                    }
                                }
                                Label {
                                    id: humidity_text_id
                                    preferredWidth: 768/2 - 128/2 - 20
                                    layoutProperties: AbsoluteLayoutProperties {
                                        positionY: 30.0
                                        positionX: 20.0
                                    }
                                    text: ListItemData.humidity;
                                    verticalAlignment: VerticalAlignment.Center
                                    textStyle.textAlign: TextAlign.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.BodyText
                                        color: Color.White
                                    }
                                }
                            }
                            Container{
                                id: wind_direction
                                horizontalAlignment: HorizontalAlignment.Left
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                Container{
                                    layout: AbsoluteLayout {
                                    }
                                    Label { 
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 0.0
                                        }
                                        id: wind_directory_description
                                        preferredWidth: 768/2 - 128/2 
                                        text: Qt.Config.tr("Wind direction")
                                        horizontalAlignment: HorizontalAlignment.Center
                                        verticalAlignment: VerticalAlignment.Center
                                        textStyle.textAlign: TextAlign.Left
                                        textStyle {
                                            base: SystemDefaults.TextStyles.SmallText
                                            color: Color.Gray
                                        }
                                    }
                                    Label {
                                        id: wind_direction_text_id
                                        preferredWidth: 768/2 - 128/2 
                                        layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 30.0
                                        }
                                        text:  ListItemData.wind_direction
                                        textStyle.textAlign: TextAlign.Left
                                        textStyle {
                                            base: SystemDefaults.TextStyles.BodyText
                                            color: Color.White
                                        }
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
                                positionY: 360.0
                            }
                            Container{
                                layout: AbsoluteLayout {
                                }
                                id: pressure
                                horizontalAlignment: HorizontalAlignment.Left
                                preferredWidth: 768/2
                                Label { 
                                    layoutProperties: AbsoluteLayoutProperties {
                                        positionY: 0.0
                                        positionX: 20.0
                                    }
                                    id: pressure_description
                                    preferredWidth: 768/2 - 128/2 - 20
                                    text: Qt.Config.tr("Pressure")
                                    horizontalAlignment: HorizontalAlignment.Center
                                    verticalAlignment: VerticalAlignment.Center
                                    textStyle.textAlign: TextAlign.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Color.Gray
                                    }
                                }
                                Label {
                                    id: pressure_text_id
                                    text: ListItemData.pressure
                                    preferredWidth: 768/2 - 128/2 - 20
                                    layoutProperties: AbsoluteLayoutProperties {
                                        positionY: 30.0
                                        positionX: 20.0
                                    }
                                    textStyle.textAlign: TextAlign.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.BodyText
                                        color: Color.White
                                    }
                                }
                            }
                            Container{
                                id: wind_speed
                                horizontalAlignment: HorizontalAlignment.Left
                                layout: AbsoluteLayout {
                                }
                                Label { 
                                    layoutProperties: AbsoluteLayoutProperties {
                                        positionY: 0.0
                                    }
                                    id: wind_speed_description
                                    preferredWidth: 768/2 - 128/2 
                                    text: Qt.Config.tr("Wind speed")
                                    horizontalAlignment: HorizontalAlignment.Center
                                    verticalAlignment: VerticalAlignment.Center
                                    textStyle.textAlign: TextAlign.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.SmallText
                                        color: Color.Gray
                                    }
                                }
                                Label {
                                    id: wind_speed_text_id
                                    text: ListItemData.wind_speed
                                    preferredWidth: 768/2 - 128/2 
                                    layoutProperties: AbsoluteLayoutProperties {
                                            positionY: 30.0
                                    }
                                    textStyle.textAlign: TextAlign.Left
                                    textStyle {
                                        base: SystemDefaults.TextStyles.BodyText
                                        color: Color.White
                                    }
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
                           id: left_arrow_id
                           visible: ListItemData.left_arrow;
                           horizontalAlignment: HorizontalAlignment.Left
                           verticalAlignment: VerticalAlignment.Center
                           preferredWidth: 62*1.6
                           preferredHeight: 62*1.6
                           defaultImageSource: "asset:///share/images/arrow_left.png"
                           onClicked: {
                                Qt.Config.prevstation();
                                Qt.main.updatestationname();
                           }
                    }
                    Container{
                        layout: DockLayout {}
                        preferredWidth: 600 
                        horizontalAlignment: HorizontalAlignment.Center 
                        Label {                 
                            id: stationname
                            horizontalAlignment: HorizontalAlignment.Center
                            textStyle {
                                base: SystemDefaults.TextStyles.BigText
                                color: Color.White
                            }
                            text: ListItemData.stationname
                        }
                    }
                    ImageButton {                 
                       id: right_arrow_id
                       visible: ListItemData.right_arrow;
                       verticalAlignment: VerticalAlignment.Center
                       horizontalAlignment: HorizontalAlignment.Right
                       defaultImageSource: "asset:///share/images/arrow_right.png"
                       preferredWidth: 62*1.6
                       preferredHeight: 62*1.6
                       onClicked: {
                            Qt.Config.nextstation();
                            Qt.main.updatestationname();
                       }
                    }
                }
                ImageView {
                    id: main_icon
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 768/2 - 64 
                        positionY: 150
                    }
                    preferredWidth: 128
                    preferredHeight: 128
                   // imageSource: Config.iconspath + "/" + Config.iconset + "/" + Current.getdata(0, "pict")
                    imageSource: ListItemData.pict 
                    horizontalAlignment: HorizontalAlignment.Center
                    overlapTouchPolicy: OverlapTouchPolicy.Allow
                    touchPropagationMode: TouchPropagationMode.PassThrough
                } 
                Container{
                        horizontalAlignment: HorizontalAlignment.Left
                        preferredWidth: 768 
                        layoutProperties: AbsoluteLayoutProperties {
                            positionY: 110.0
                        }
                        Label {
                            text: Qt.Current.getdata(0, "current") == true ? Qt.Config.tr("Now") : Qt.Config.tr("Today")
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Center
                            textStyle.textAlign: TextAlign.Center
                            textStyle {
                                base: SystemDefaults.TextStyles.SmallText
                                color: Color.White
                            }
                        }
                }





                                    }
                                    Container{
                                    layout: DockLayout {
                                    }
                                    visible: ListItemData.number == 0 ? false : true;
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
                                        Container{
                                            layout: AbsoluteLayout {
                                            }

                                            Label {
                                                text: ListItemData.fulldate
                                                textStyle.textAlign: TextAlign.Left
                                                preferredWidth: 768/2 - 128/2 - 20
                                                textStyle {    
                                                    base: SystemDefaults.TextStyles.SmallText
                                                    color: Color.Gray
                                                }
                                            }
                                            Label {
                                                text: ListItemData.shortdate
                                                preferredWidth: 768/2 - 128/2 - 20
                                                layoutProperties: AbsoluteLayoutProperties {
                                                    positionY: 30.0
                                                }
                                                textStyle.textAlign: TextAlign.Left
                                                textStyle {
                                                    base: SystemDefaults.TextStyles.BodyText
                                                    color: Color.White
                                                }
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
                                            onCreationCompleted: {
                                                console.log("ppppppppppppppppppppppppppp\n");
                                            }
                                        }
                                        Container{
                                            preferredWidth: 20
                                            maxWidth: 20
                                        }
                                    }                                                                                                   
                                } 
                            }
                            }
                        ]
                        onTriggered: {             
                            console.log("Index ", groupDataModel.data(indexPath).number);
                            main.day = groupDataModel.data(indexPath).number;
                            main.day_period = "day";
                            main.current = false;
                            var newPage = fullpageDefinition.createObject();
                            rootWindow.push(newPage);
                        }

                                                }
                }
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    preferredWidth: 768 
                    visible : Config.stationname == "Unknown" ? true : false;
                    text: Config.tr("Set locations")
                    onClicked: {
                        var newPage = sourcepageDefinition.createObject();
                        rootWindow.push(newPage);
                    }
                    layoutProperties: AbsoluteLayoutProperties {
                        positionY: 800 
                    }
                }

            }
        }
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0 
                positionY: 1142
            }
            preferredWidth: 768 
            preferredHeight: 138 
            background: Color.create("#1f1f1f")
            Container{    
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                Container{
                    preferredWidth: 20
                }
                ActivityIndicator {
                    id: refresh_showing
                    verticalAlignment: VerticalAlignment.Center
                    visible: main.isUpdate ? true : false
                    running: main.isUpdate ? true : false
                    enabled: main.isUpdate ? true : false
                }
                ImageButton {
                    id: refreshicon
                    verticalAlignment: VerticalAlignment.Center
                    defaultImageSource: "asset:///button_icons/icon_refresh.png"
                    visible: main.isUpdate ? false : true
                    onClicked: {
                        main.isUpdate = true;
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
                    onTouch: {
                        if (event.isDown()) {
                            Config.showweb();
                        }
                    }
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
}
