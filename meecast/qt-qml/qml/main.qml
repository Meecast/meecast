import bb.cascades 1.0    


NavigationPane {
    
    property int main_icon_size : 440 
    property int row_icon_size : 90
    property color current_rect_back_background :  "Red" 
    property string feels_like_temp_text : ""
    property string main_icon_imageSource : "" 
    property string title_text_text : ""
    property string current_temp_text: ""
    property string humidity_text: "" 
    property string pressure_text: ""
    property string wind_speed_text: ""
    property string wind_direction_text: ""    
    property bool  left_arrow : false
    property bool  right_arrow : false
    property int wind_direction_angle : 0
    property bool current_value: false
    property int main_information_position : 150 
    property int screen_height : Config.get_height_resolution() 
    property int screen_width : Config.get_width_resolution() 
    property int menu_height : Config.get_height_resolution() == 1280 ? 138 : 110;
    backButtonsVisible: false
    // Create the initial screen
    property variant newPage;
    id: rootWindow 
    onPopTransitionEnded: {
        if (newPage == page) {
           /* console.log("onPopTransitionEnded"); */
            rootWindow.backButtonsVisible = false;
            page.destroy();
        }
    }
    Page {
        id: main
        property int day: 0
        property bool current: false
        property string day_period: "day"
        property bool isUpdate: false

        property string fulldate: ""
        property string shortdate: ""
        property string temp_high: ""
        property string temp_low: ""
        property string hi_temp_color: ""
        property string pict: ""
        property string bg_color: ""
        property string title_text: ""
        property string wind_direction: ""
        property string wind_speed: ""
        property string humidity: ""
        property string pressure: ""
        property string background_image: ""

        function check_hours ()
        {
            var i = 0;
            var result = 0;
            console.debug ("Forecast_model.rowCount()", Forecast_model.rowCount(), Current.rowCount());
            var fulldate = Forecast_model.getdata(day, "fulldate");
            while (i<Forecast_hours_model.rowCount()){   
                if (Forecast_hours_model.getdata(i, "fulldate") == fulldate)
                    result = 1;
                i++;
            }
            return result;
        }


        function update_list(){
            forrecasts_list.dataModel.clear();
            if (Current.getdata(0, "current"))
                current_value = Current.getdata(0, "current");
            else
                current_value = false;
     /*       console.log("Forecast_model.rowCount() ",  Forecast_model.rowCount()); */
     /*       console.log("Cuurent.rowCount() ",  Current.rowCount()); */
            for (var a = 0; a < Forecast_model.rowCount() + 1; a++) {
                if (a == 0){
                    if (Current.rowCount() > 0){
                        fulldate = Current.getdata(a, "fulldate");
                        shortdate = Current.getdata(a, "shortdate");
                        temp_high = Current.getdata(a, "temp_high");
                        temp_low  = Current.getdata(a, "temp_low");
                        if (Current.getdata(a, "temp_high") != "N/A"){
                            hi_temp_color =  main.getColor(Current.getdata(a, "temp_high"));
                        }else{
                            hi_temp_color = "#000000";
                        }    
                    }else{
                        fulldate = "";
                        shortdate = "";
                        temp_high = "";
                        temp_low  = "";
                        hi_temp_color = ""; 
                    }
                    pict = main_icon_imageSource;
                    bg_color = current_rect_back_background;
                    title_text = title_text_text;
                    wind_direction = wind_direction_text;
                    wind_speed = wind_speed_text;
                    humidity = humidity_text;
                    pressure = pressure_text;
                    background_image = "asset:///share/images/mask_background_main.png"

                }else{
                    fulldate = Forecast_model.getdata(a-1, "fulldate");
                    shortdate = Forecast_model.getdata(a-1 , "shortdate");
                    temp_high = Forecast_model.getdata(a-1, "temp_high");
                    temp_low  = Forecast_model.getdata(a-1, "temp_low");
                    if (Forecast_model.getdata(a-1, "temp_high") != "N/A"){
                        hi_temp_color =  main.getColor(Forecast_model.getdata(a-1, "temp_high"));
                    }else{
                        if (Forecast_model.getdata(a-1, "temp") != "N/A"){
                            hi_temp_color =  main.getColor(Forecast_model.getdata(a-1, "temp"));
                        }
                    }
                    pict = Config.iconspath + "/" + Config.iconset + "/" + Forecast_model.getdata(a-1, "pict");
                    bg_color = ((a-1) % 2 != 0) ? "#1f1f1f" : "#000000";
                    title_text = Forecast_model.getdata(a-1, "description");
                    wind_direction = Forecast_model.getdata(a-1, "wind_direction");
                    wind_speed = Forecast_model.getdata(a-1, "wind_speed");
                    humidity = Forecast_model.getdata(a-1, "humidity");
                    pressure = Forecast_model.getdata(a-1, "pressure");
                    background_image = "";
                }
                if (a == 1){
            //        bg_color = current_rect_back_background;
            //        background_image = "asset:///share/images/row_background.png"
                    shortdate = Config.tr("Today");
                }
                forrecasts_list.dataModel.insert(
                    {
                     "fulldate" : fulldate,
                     "shortdate" : shortdate, 
                     "pict" : pict,
                     "temp_high" : temp_high,
                     "temp_low" : temp_low,
                     "bg_color" : bg_color ,
                     "hi_temp_color" :  hi_temp_color,
                     "wind_speed" : wind_speed, 
                     "number" : a,
                     "description_text" : title_text,
                     "wind_direction" : wind_direction, 
                     "stationname" : Config.stationname == "Unknown" ? "MeeCast" : Config.stationname,
                     "current_temp_text" : current_temp_text,
                     "humidity" : humidity, 
                     "pressure" : pressure, 
                     "wind_direction_angle" : wind_direction_angle,
                     "left_arrow" : left_arrow,
                     "right_arrow" : right_arrow,
                     "stationname" : Config.stationname == "Unknown" ? "MeeCast" : Config.stationname,
                     "current": current_value,
                     "background_image": background_image
                    }
                )
            }
        }

        function update_current_data(){
            //console.log("update_current_data!!!!!!!!!!!!!");
            left_arrow = Config.prevstationname == "" ? false : true;
            right_arrow = Config.nextstationname == "" ? false : true;
            if (Current.getdata(0, "temp") == "N/A"){
                current_temp_text = ""
                if (Current.getdata(0, "temp_high") != "N/A")
                   current_temp_text = Current.getdata(0, "temp_high") + '°'
                if ((Current.getdata(0, "temp_low") != "N/A") && (Current.getdata(0, "temp_high") != "N/A"))
                if (Current.getdata(0, "temp_low") != "N/A")
                   current_temp_text = current_temp_text + '/'+ Current.getdata(0, "temp_low") + '°'
              //     console.log("Color!!!! ", Current.getdata(0, "temp_high"));
                if (Current.getdata(0, "temp_high") != "N/A"){
                    current_rect_back_background = main.getColor(Current.getdata(0, "temp_high"));
                }else{
                //   console.log("Color!!!! ", Current.getdata(0, "temp_low"));
                    if (Current.getdata(0, "temp_low") != "N/A"){
                        current_rect_back_background = main.getColor(Current.getdata(0, "temp_high"));
                    }else{
                        current_rect_back_background = "#000000";
                    }
                }
            }else{
               current_temp_text = Current.getdata(0, "temp") + '°'
               console.log(Current.getdata(0, "temp"));
               if (Current.getdata(0, "temp") > -273){
                   current_rect_back_background = main.getColor(Current.getdata(0, "temp"));
               }
               
            }
            main_icon_imageSource = Config.iconspath + "/" + Config.iconset + "/" + Current.getdata(0, "pict")
            title_text_text = Config.tr(Current.getdata(0, "description"));
            if (Current.getdata(0, "humidity") != "N/A"){
                humidity_text = Current.getdata(0, "humidity") + " %";
            }else
                humidity_text = "N/A";
            pressure_text = Current.getdata(0, "pressure") + ' ' + Config.tr(Config.pressureunit) ;

            if (Current.getdata(0, "wind_speed") != "N/A"){ 
                wind_speed_text = Current.getdata(0, "wind_speed") + ' ' + Config.tr(Config.windspeedunit);
            }else
                wind_speed_text = "N/A";
            if (Current.getdata(0, "wind_direction") != "N/A" && Current.getdata(0, "wind_direction") != undefined){
                wind_direction_angle =  main.getAngle(Current.getdata(0, "wind_direction"));
            }else
                wind_direction_angle = 0;
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
            if (s == "CALM")
               return -1; 
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
        //    console.log("updatestationname() ", Config.stationname );
            main.updatemodels();
            //stationname_text = Config.stationname == "Unknown" ? "MeeCast" : Config.stationname

            sourceicon.visible = false;
            sourceicon.imageSource = Config.imagespath + "/" + Config.source + ".png";
            sourceicon.visible = true;
        }


        function onConfigChanged() {
            console.log("end update station name = " + Config.stationname);
            startview.visible = Config.stationname == "Unknown" ? true : false;
//            mainview.visible = Config.stationname == "Unknown" ? false : true;
            main.updatestationname();
            main.updatemodels();
            main.isUpdate = false;

            if ( Config.stationname != "Unknown" && Forecast_model.rowCount() != 0 || Current.rowCount() != 0)
            { 
                refresh_button.visible = false;
            }
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
            if (Config.stationname == "Unknown" || (Forecast_model.rowCount() == 0 &&  Current.rowCount() == 0)){
                forrecasts_list.visible = false;
                if ((Config.stationname != "Unknown" && Forecast_model.rowCount() == 0 && Current.rowCount() == 0)){
                    refresh_button.visible = true;
                    forrecasts_list.visible = false;
                    notrefreshview.visible = true;
                }         
            }else{
                forrecasts_list.visible = true;
                notrefreshview.visible = false;
            }
            main.update_list();
            main.update_current_data();
            
            
            //list.height = 80 * Forecast_model.rowCount();
            //dataview.visible = (Forecast_model.rowCount() == 0 || Current.rowCount() == 0) ? true : false;
            //current_rect.visible = Current.rowCount() == 0 ? false : true;
            //list.visible = (Forecast_model.rowCount() == 0) ? false : true;
        }

   
        content:
            Container {
                layout: AbsoluteLayout {}
                Container {
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
                        preferredWidth: screen_width
                        preferredHeight: screen_height - 280
                    }

                    Container{
                        id: startview
                        visible : Config.stationname == "Unknown" ? true : false;
                        preferredWidth: screen_width
                        layout: AbsoluteLayout {}
                        Container{
                            preferredWidth: screen_width
                            preferredHeight: 438
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 0
                            }
                            background: Color.White

                        }
                        ImageView {
                            imageSource: "asset:///share/images/mask_background.png"
                            preferredWidth: screen_width
                            preferredHeight: 438
                        }
                        Label {
                            text: Config.tr("No locations are set up yet.") 
                            horizontalAlignment: HorizontalAlignment.Center
                            preferredWidth: screen_width 
                            multiline: true
                            textStyle.textAlign: TextAlign.Center
                            textStyle {
                                    base: SystemDefaults.TextStyles.BigText
                                    color: Color.create("#999999")
                            }
                            layoutProperties: AbsoluteLayoutProperties {
//                                positionY: 400 
                                positionY: screen_height / 3 
                            }
                        }
                    }
                    Container{
                        id: notrefreshview
                        visible : (  Config.stationname != "Unknown" && Forecast_model.rowCount() == 0 && Current.rowCount() == 0) ? true : false
                        preferredWidth: screen_width
                        layout: AbsoluteLayout {
                        }
                        Container{
                            preferredWidth: screen_width
                            preferredHeight: 438
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 0
                            }
                            background: Color.White

                        }
                        ImageView {
                            imageSource: "asset:///share/images/mask_background.png"
                            preferredWidth: screen_width
                            preferredHeight: 438
                            layoutProperties: AbsoluteLayoutProperties {
                                positionY: 0
                            }
                        }
                        Label {
                            text: Config.tr("Looks like there's no info for this location.")
                            horizontalAlignment: HorizontalAlignment.Center
                            preferredWidth: screen_width 
                            multiline: true
                            textStyle.textAlign: TextAlign.Center
                            textStyle {
                                    base: SystemDefaults.TextStyles.BigText
                                    color: Color.create("#999999")
                            }
                            layoutProperties: AbsoluteLayoutProperties {
//                                positionY: 400 
                                positionY: screen_height / 3 
                            }
                        }
                    }
                    Container{
                        layoutProperties: AbsoluteLayoutProperties {
                            positionX: 0
                            positionY: 0
                        }
                        layout: AbsoluteLayout {}
                        preferredWidth: screen_width
                        preferredHeight: screen_height - menu_height 
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
                                Qt.main_icon_size = main_icon_size
                                Qt.main_information_position = main_information_position
                                Qt.row_icon_size = row_icon_size
                                Qt.screen_width = screen_width  
                                Qt.screen_height = screen_height
                                main.update_list();
                            }

                            listItemComponents: [
                                ListItemComponent {
                                    type: "item"
                                    Container{
                                        Container{
                                            layout: AbsoluteLayout {}
        //                                    visible: ListItemData.number == 0 && ListItemData.current_value == true ? true : false;
                                            visible: ListItemData.number == 0  ? true : false;
                                            Container{
                                                preferredWidth: Qt.screen_width
                                                layoutProperties: AbsoluteLayoutProperties {
                                                    positionX: 0
                                                    positionY: 0 
                                                }
                                                id: current_rect
                                                //visible: Current.rowCount() == 0 ? false : true
                                                layout: AbsoluteLayout {
                                                }
                                                Container{
                                                    id: current_rect_back
                                                    visible: ListItemData.fulldate != "" ? true :false
                                                    preferredWidth: Qt.screen_width
                                                    preferredHeight: 564 
                                                    layoutProperties: AbsoluteLayoutProperties {
                                                        positionY: 0
                                                    }
                                                    background: Color.create(ListItemData.bg_color)
                                                }
                                                ImageView {
                                                    imageSource: ListItemData.background_image
                                                    visible: ListItemData.fulldate != "" ? true :false
                                                    preferredWidth: Qt.screen_width
                                                    preferredHeight: 564 
                                                    layoutProperties: AbsoluteLayoutProperties {
                                                        positionY: 0
                                                    }
                                                }         
                                                Container{
                                                    preferredWidth: Qt.screen_width
                                                    visible: ListItemData.fulldate != "" ? true :false
                                                    id: title_day_and_other_paremeters
                                                    preferredHeight: Qt.main_icon_size - Qt.main_information_position + 50
                                                    layout: DockLayout {}
                                                    layoutProperties: AbsoluteLayoutProperties {
                                                        positionY: Qt.main_information_position 
                                                    }
                                                    Container{
                                                        horizontalAlignment: HorizontalAlignment.Left
                                                        preferredWidth: Qt.screen_width/2 - Qt.main_icon_size/2 + 30       
                                                        Container{
                                                            horizontalAlignment: HorizontalAlignment.Left
                                                            layout: AbsoluteLayout {} 
                                                            Label {
                                                                layoutProperties: AbsoluteLayoutProperties {
                                                                    positionX: 10.0
                                                                    positionY: 0.0
                                                                }
                                                                visible: ListItemData.fulldate != "" ? true :false
                                                                text: ListItemData.current == true ? Qt.Config.tr("Now") : Qt.Config.tr("Today")
                                                                horizontalAlignment: HorizontalAlignment.Left
                                                                verticalAlignment: VerticalAlignment.Center
                                                                textStyle.textAlign: TextAlign.Left
                                                                textStyle {
                                                                    fontSize: FontSize.PointValue
                                                                    fontWeight: FontWeight.W100
                                                                    fontSizeValue: 7.0
                                                                    color: Color.Gray
                                                                }
                                                            }
                                                        }
                                                    }
                                                    Container{
                                                        id: humidity_wind_pressure_id 
                                                        horizontalAlignment: HorizontalAlignment.Left
                                                        preferredWidth: Qt.screen_width/2 - Qt.main_icon_size/2 + 60
                                                        verticalAlignment: VerticalAlignment.Center

                                                        Container{
                                                            layout: AbsoluteLayout {} 
                                                            horizontalAlignment: HorizontalAlignment.Left
                                                            Container{
                                                                id: humidity
                                                                horizontalAlignment: HorizontalAlignment.Left
                                                                visible: ListItemData.humidity == "N/A" ? false : true; 
                                                                layoutProperties: AbsoluteLayoutProperties {
                                                                    positionX: 10.0
                                                                    positionY: 0.0
                                                                }

                                                                layout: AbsoluteLayout {
                                                                }
                                                                ImageView {
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionY: 8.0
                                                                    }
                                                                    imageSource: "asset:///share/images/humidity.png"
                                                                    preferredWidth: 30
                                                                    preferredHeight: 30
                                                                }
                                                                Label {
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionX: 40.0
                                                                        positionY: 0.0
                                                                    }

                                                                    id: humidity_text
                                                                    text: ListItemData.humidity;
                                                                    verticalAlignment: VerticalAlignment.Center
                                                                    textStyle {
                                                                        fontSize: FontSize.PointValue
                                                                        fontWeight: FontWeight.W100
                                                                        fontSizeValue: 7.0
                                                                        color: Color.White
                                                                    }
                                                                }
                                                            }
                                                            Container{
                                                                id: wind_direction
                                                                layoutProperties: AbsoluteLayoutProperties {
                                                                    positionX: 10.0
                                                                    positionY: 0.0 + (ListItemData.humidity == Qt.Config.tr("N/A") || ListItemData.humidity == "N/A") ? 0.0 : 60.0; 
                                                                }
                                                                visible: ListItemData.wind_direction == Qt.Config.tr("N/A") ? false : true; 
                                                                //visible: false
                                                                layout: AbsoluteLayout {} 
                                                                horizontalAlignment: HorizontalAlignment.Left
                                                                Container{
                                                                    layout: AbsoluteLayout {
                                                                    }
                                                                    ImageView {
                                                                        layoutProperties: AbsoluteLayoutProperties {
                                                                            positionX: 0.0
                                                                            positionY: 8.0
                                                                        }
                                                                        imageSource: "asset:///share/images/wind_direction_background.png"
                                                                        preferredWidth: 30
                                                                        preferredHeight: 30
                                                                    }
                                                                    ImageView {
                                                                        id: wind_direction_arrow
                                                                        layoutProperties: AbsoluteLayoutProperties {
                                                                            positionX: 0.0
                                                                            positionY: 8.0
                                                                        }
                                                                        visible: ListItemData.wind_direction_angle == -1 ? false : true;
                                                                        imageSource: "asset:///share/images//wind_direction_arrow.png"
                                                                        preferredWidth: 30
                                                                        preferredHeight: 30
                                                                        rotationZ: ListItemData.wind_direction_angle  
                                                                    }
                                                                }
                                                                Label {
                                                                    id: wind_direction_text
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionX: 40.0
                                                                        positionY: 0.0
                                                                    }
                                                                    text: ListItemData.wind_direction
                                                                    textStyle {
                                                                        fontWeight: FontWeight.W100
                                                                        fontSize: FontSize.PointValue
                                                                        fontSizeValue: 7.0
                                                                        color: Color.White
                                                                    }
                                                                }
                                                            }        
                                                            Container{
                                                                id: wind_speed 
                                                                visible: ListItemData.wind_speed == "N/A" ? false : true; 
                                                                verticalAlignment: VerticalAlignment.Center
                                                                horizontalAlignment: HorizontalAlignment.Left
                                                                layoutProperties: AbsoluteLayoutProperties {
                                                                    positionX: 10.0
                                                                    positionY: 0.0 + ((ListItemData.humidity == Qt.Config.tr("N/A") || ListItemData.humidity == "N/A") ? 0.0 : 60.0) + ((ListItemData.wind_direction == Qt.Config.tr("N/A") || ListItemData.wind_direction == "N/A")  ? 0.0 : 60.0); 
                                                                }

                                                                layout: AbsoluteLayout {}
                                                                ImageView {
                                                                    imageSource: "asset:///share/images/wind_speed.png"
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionY: 8.0
                                                                    }
                                                                    verticalAlignment: VerticalAlignment.Center
                                                                    preferredWidth: 30
                                                                    preferredHeight: 30
                                                                }
                                                                Label {
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionX: 40.0
                                                                        positionY: 0.0
                                                                    }
                                                                    id: wind_speed_text
                                                                    text: ListItemData.wind_speed;
                                                                    verticalAlignment: VerticalAlignment.Center
                                                                    textStyle {
                                                                        fontSize: FontSize.PointValue
                                                                        fontWeight: FontWeight.W100
                                                                        fontSizeValue: 7.0
                                                                        color: Color.White
                                                                    }
                                                                }
                                                            }
                                                            Container{
                                                                id: pressure 
                                                                visible: ListItemData.pressure.search("N/A") != -1  ? false : true; 
                                                                verticalAlignment: VerticalAlignment.Center
                                                                horizontalAlignment: HorizontalAlignment.Left
                                                                layoutProperties: AbsoluteLayoutProperties {
                                                                    positionX: 10.0
                                                                    positionY: 0.0 + ((ListItemData.wind_speed == Qt.Config.tr("N/A") || ListItemData.wind_speed == "N/A") ? 0.0 : 60.0) + ((ListItemData.humidity == Qt.Config.tr("N/A") || ListItemData.humidity == "N/A") ? 0.0 : 60.0) + ((ListItemData.wind_direction == Qt.Config.tr("N/A") || ListItemData.wind_direction == "N/A")  ? 0.0 : 60.0); 
                                                                }

                                                                layout: AbsoluteLayout {}
                                                                ImageView {
                                                                    imageSource: "asset:///share/images/pressure.png"
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionY: 8.0
                                                                    }
                                                                    verticalAlignment: VerticalAlignment.Center
                                                                    preferredWidth: 30
                                                                    preferredHeight: 30
                                                                }
                                                                Label {
                                                                    layoutProperties: AbsoluteLayoutProperties {
                                                                        positionX: 40.0
                                                                        positionY: 0.0
                                                                    }
                                                                    id: pressure_text
                                                                    text: ListItemData.pressure;
                                                                    verticalAlignment: VerticalAlignment.Center
                                                                    textStyle {
                                                                        fontSize: FontSize.PointValue
                                                                        fontWeight: FontWeight.W100
                                                                        fontSizeValue: 7.0
                                                                        color: Color.White
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    } 
                                                }
                                                Container{
                                                    id: title
                                                    preferredWidth: Qt.screen_width
                                                    layout: DockLayout {}
                                                    layoutProperties: AbsoluteLayoutProperties {
                                                        positionY: 480.0
                                                    }
                                                    Label {                 
                                                        id: title_text
                                                        text: ListItemData.description_text 
                                                        horizontalAlignment: HorizontalAlignment.Center
                                                        textStyle {
                                                            fontSize: FontSize.PointValue
                                                            fontSizeValue: 8.0
                                                            fontWeight: FontWeight.W100
                                                            color: Color.White
                                                        }
                                                    }
                                                }
                                            }
                                            Container{
                                                id: topStringWithStationName
                                                preferredWidth: Qt.screen_width
                                                layout: DockLayout {}
                                                layoutProperties: AbsoluteLayoutProperties {
                                                    positionX: 0
                                                    positionY: 0
                                                }
                                                Container{
                                                    preferredWidth: 20 
                                                }
                                                ImageButton {                 
                                                   id: left_arrow_id
                                                   visible: ListItemData.left_arrow;
                                                   horizontalAlignment: HorizontalAlignment.Left
                                                   verticalAlignment: VerticalAlignment.Center
                                                   preferredWidth: 60
                                                   preferredHeight: 60
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
                                                    verticalAlignment: VerticalAlignment.Center 
                                                    Label {                 
                                                        id: stationname
                                                        horizontalAlignment: HorizontalAlignment.Center
                                                        textStyle {
                                                            fontSize: FontSize.PointValue
                                                            fontSizeValue: 8.0
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
                                                   preferredWidth: 60
                                                   preferredHeight: 60
                                                   onClicked: {
                                                       Qt.Config.nextstation();
                                                       Qt.main.updatestationname();
                                                   }
                                               }
                                               Container{
                                                   preferredWidth: 20 
                                               }
                                            }
                                            ImageView {
                                                id: main_icon
                                                visible: ListItemData.fulldate != "" ? true : false
                                                layoutProperties: AbsoluteLayoutProperties {
                                                    positionX: Qt.screen_width/2 - Qt.main_icon_size/2 
                                                    positionY: 40 
                                                }
                                                preferredWidth: Qt.main_icon_size 
                                                preferredHeight: Qt.main_icon_size 
                                                imageSource: ListItemData.pict 
                                                horizontalAlignment: HorizontalAlignment.Center
                                                overlapTouchPolicy: OverlapTouchPolicy.Allow
                                                touchPropagationMode: TouchPropagationMode.PassThrough
                                            } 
                                            Container{
                                                id: current_temperature
                                                horizontalAlignment: HorizontalAlignment.Right
                                                preferredWidth: Qt.screen_width
                                                verticalAlignment: VerticalAlignment.Bottom
                                                layout: AbsoluteLayout {}
                                                Container {
                                                   preferredWidth: Qt.screen_width/2
                                                   layoutProperties: AbsoluteLayoutProperties {
                                                       positionX: Qt.screen_width/2
                                                       positionY: 360
                                                   }
                                                   Label {                                                               
                                                       id: current_temp_text_id
                                                       text: ListItemData.current_temp_text                                                                
                                                       horizontalAlignment: HorizontalAlignment.Center
                                                       verticalAlignment: VerticalAlignment.Bottom
                                                       textStyle.textAlign: TextAlign.Center
                                                       textStyle {
                                                           fontSize: FontSize.PointValue
                                                           fontWeight: FontWeight.W100
                                                           fontSizeValue: ListItemData.current_temp_text.length < 5 ? 26.0 : 20 
                                                           color: Color.White
                                                       }
                                                   }
                                                }
                                            }
                                        }
                                        Container{
                                            visible: ListItemData.number == 0 ? false : true;
                                            background: Color.create(ListItemData.bg_color)
                                            preferredWidth: Qt.screen_width
                                            preferredHeight: 120 
                                            Container{
                                                visible: ListItemData.background_image != "" ? true : false;
                                                ImageView {
                                                     imageSource: ListItemData.background_image
                                                     horizontalAlignment: HorizontalAlignment.Center  
                                                }
                                            }
                                            layout: AbsoluteLayout {}
                                            Container{
                                                layout: DockLayout {}
                                                preferredWidth: Qt.screen_width
                                                preferredHeight: 120 
                                                Container{
                                                    layout: StackLayout {
                                                        orientation: LayoutOrientation.LeftToRight
                                                    }
                                                    preferredWidth: Qt.screen_width/2
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
                                                            preferredWidth: Qt.screen_width/2 - Qt.row_icon_size/2 - 20
                                                            textStyle {    
                                                                base: SystemDefaults.TextStyles.BodyText 
                                                                fontWeight: FontWeight.W100
                                                                color: Color.Gray
                                                            }
                                                        }
                                                        Label {
                                                            text: ListItemData.shortdate
                                                            preferredWidth: Qt.screen_width/2 - Qt.row_icon_size/2 - 20
                                                            layoutProperties: AbsoluteLayoutProperties {
                                                                positionX: 70.0
                                                            }
                                                            textStyle.textAlign: TextAlign.Left
                                                            textStyle {
                                                                base: SystemDefaults.TextStyles.BodyText
                                                                fontWeight: FontWeight.W100
                                                                color: Color.White
                                                            }
                                                        }
                                                    }
                                                }
                                                ImageView {
                                                     imageSource: ListItemData.pict
                                                     preferredWidth: Qt.row_icon_size
                                                     preferredHeight: Qt.row_icon_size
                                                     horizontalAlignment: HorizontalAlignment.Center                
                                                     verticalAlignment: VerticalAlignment.Center                
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
                                                        preferredWidth: 80 
                                                        textStyle.textAlign: TextAlign.Right
                                                        textStyle {
                                                            base: SystemDefaults.TextStyles.BodyText
                                                            fontWeight: FontWeight.W100
                                                            color: Color.create(ListItemData.hi_temp_color)
                                                        }
                                                    }
                                                    Label {
                                                        text: ListItemData.temp_low + '°'
                                                        horizontalAlignment: HorizontalAlignment.Right
                                                        preferredWidth: 80
                                                        textStyle.textAlign: TextAlign.Right
                                                        textStyle {
                                                            base: SystemDefaults.TextStyles.BodyText
                                                            fontWeight: FontWeight.W100
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
                                    }
                                }
                            ]
                            onTriggered: {             
                                if ( groupDataModel.data(indexPath).number == 0 ){
                                    console.log("Index ", groupDataModel.data(indexPath).number);
                                    main.day = groupDataModel.data(indexPath).number;
                                    main.current = true;
                                }else{
                                    console.log("Index ", (groupDataModel.data(indexPath).number - 1));
                                    main.day = groupDataModel.data(indexPath).number - 1;
                                    main.current = false;
                                }

                                //mySheet.open();
                                console.log("DAY    ", main.day);

                                main.day_period = "day";
                                var newPage = fullpageDefinition.createObject();
                                rootWindow.push(newPage);
                                
                               // day_tab.setContent(newPage1);

                               // main.day_period = "night";
                               // var newPage2 = fullpageDefinition.createObject();
                               // night_tab.setContent(newPage2);

                               // if (main.check_hours() > 0){
                               //     tabFullWeather.add(hourly_tab);
                               //     main.day_period = "hours";
                               //     var newPage4 = fullpageDefinition.createObject();
                               //     hourly_tab.setContent(newPage4);
                               // }else{
                               //     tabFullWeather.remove(hourly_tab);
                               // }

                               // if (main.current == true &&  current_value == true){
                               //     tabFullWeather.insert(1, current_tab);
                               //     main.day_period = "current";
                               //     var newPage3 = fullpageDefinition.createObject();
                               //     current_tab.setContent(newPage3);
                               //     tabFullWeather.activeTab = current_tab;
                               // }else{
                               //     tabFullWeather.activeTab = day_tab;
                               //     tabFullWeather.remove(current_tab);
                               // }
                            }
                        }
                    }
                    Button {
                        horizontalAlignment: HorizontalAlignment.Center
                        preferredWidth: screen_width 
                        visible : Config.stationname == "Unknown" ? true : false;
                        text: Config.tr("Set locations")
                        onClicked: {
                            var newPage = sourcepageDefinition.createObject();
                            rootWindow.push(newPage);
                        }
                        layoutProperties: AbsoluteLayoutProperties {
                            positionY: screen_height/3 + 200 
                        }
                    }
                    Button {
                        id: refresh_button
                        horizontalAlignment: HorizontalAlignment.Center
                        preferredWidth: screen_width 
                        visible : (Config.stationname != "Unknown" && Forecast_model.rowCount() == 0 && Current.rowCount() == 0) ? true : false
                        text: Config.tr("Try to update")
                        onClicked: {
                            main.isUpdate = true;
                            Config.updatestations()
                        }
                        layoutProperties: AbsoluteLayoutProperties {
                            positionY: screen_height/3 + 200 
                        }
                    }
                }
            }
            Container{

                id: toolbar
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0 
                    positionY: screen_height - menu_height 
                }
                preferredWidth: screen_width 
                preferredHeight:  menu_height 
                background: Color.create("#1f1f1f")
                Container{    
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Container{
                        preferredWidth: 1
                        minWidth:       1 
                        preferredHeight: menu_height 
                        maxHeight:       menu_height 
                        minHeight:       menu_height 
                    }
                    Container{
                        layout: AbsoluteLayout {}
                        verticalAlignment: VerticalAlignment.Center     
                        horizontalAlignment: HorizontalAlignment.Right
                        leftMargin: 10.0
                        rightMargin: 10.0
                        Container{
                            layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0 
                                positionY: 0
                            }
                            maxWidth:        110
                            minWidth:        110
                            ActivityIndicator {
                                id: refresh_showing
                                preferredWidth:  81
                                maxWidth:        81
                                minWidth:        81
                                preferredHeight: 81 
                                maxHeight:       81 
                                minHeight:       81
                                horizontalAlignment: HorizontalAlignment.Center 
                                verticalAlignment: VerticalAlignment.Center
                                visible: main.isUpdate ? true : false
                                running: main.isUpdate ? true : false
                                enabled: main.isUpdate ? true : false
                            }
                            ImageButton {
                                id: refreshicon
                                preferredWidth:  81
                                maxWidth:        81
                                minWidth:        81
                                preferredHeight: 81 
                                maxHeight:       81 
                                minHeight:       81 
                                verticalAlignment: VerticalAlignment.Center
                                horizontalAlignment: HorizontalAlignment.Center
                                defaultImageSource: "asset:///button_icons/refresh_def.png"
                                pressedImageSource: "asset:///button_icons/refresh_sel.png"
                                visible: main.isUpdate ? false : true
                                onClicked: {
                                    main.isUpdate = true;
                                    Config.updatestations()
                                }
                            }
                        }
                        Label {
                            layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0 
                                positionY: 80
                            }
                            text: Config.tr("Update") 
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Top     
                            textStyle.textAlign: TextAlign.Center
                            maxWidth:        110
                            minWidth:        110
                            textStyle {
                                fontSize: FontSize.PointValue
                                fontSizeValue: 4.0
                                fontWeight: FontWeight.W100
                                color: Color.create("#ffffff")
                            }
                        }

                    }
                    Container{
                        preferredWidth: 2
                        minWidth:       2 
                        preferredHeight: menu_height
                        maxHeight:       menu_height 
                        minHeight:       menu_height
                        background: Color.create("#000000")
                    }
                    ImageView {
                        id: sourceicon
                        verticalAlignment: VerticalAlignment.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        imageSource: Config.stationname == "Unknown" ? "" : Config.imagespath + "/" + Config.source + ".png"                 
                        preferredWidth: screen_width - 328
                        maxWidth:       screen_width - 328
                        minWidth:       screen_width - 328
                        preferredHeight: 120 
                        leftMargin: 30.0
                        rightMargin: 30.0
                        onTouch: {
                            if (event.isDown()) {
                                Config.showweb();
                            }
                        }
                    }
                    Container{
                        preferredWidth: 2 
                        minWidth:       2 
                        preferredHeight: menu_height
                        maxHeight:       menu_height 
                        minHeight:       menu_height
                        background: Color.create("#000000")
                    }
                   
                    Container{
                        layout: AbsoluteLayout {}
                        verticalAlignment: VerticalAlignment.Center     
                        horizontalAlignment: HorizontalAlignment.Right
                        leftMargin: 10.0
                        rightMargin: 10.0
                        Container{
                            layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0 
                                positionY: 0
                            }
                            maxWidth: 110
                            minWidth: 110
                            ImageButton {
                                id: settingsicon
                                horizontalAlignment: HorizontalAlignment.Center
                                verticalAlignment: VerticalAlignment.Top    
                                defaultImageSource: "asset:///button_icons/menu_def.png"
                                pressedImageSource: "asset:///button_icons/menu_sel.png"
                                preferredWidth:  81
                                maxWidth:        110
                                minWidth:        81
                                preferredHeight: 81 
                                maxHeight:       81 
                                minHeight:       81 
                                onClicked: {
                                    newPage = settingspageDefinition.createObject();
                                    rootWindow.push(newPage);
                                }
                            }
                        } 
                        Label {
                            layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0 
                                positionY: 80
                            }
                            text: Config.tr("Settings") 
                            horizontalAlignment: HorizontalAlignment.Center
                            verticalAlignment: VerticalAlignment.Top     
                            textStyle.textAlign: TextAlign.Center
                            maxWidth:        110
                            minWidth:        110
                            textStyle {
                                fontSize: FontSize.PointValue
                                fontSizeValue: 4.0
                                fontWeight: FontWeight.W100
                                color: Color.create("#ffffff")
                            }
                        }
                    }
                }
            }
        }
        attachedObjects: [
            Tab {
                id: current_tab
                title: Config.tr("Now")
                imageSource: Config.imagespath +  "/now_def.png" 
            },
            Tab {
                id: hourly_tab
                title: Config.tr("Hourly")
                imageSource: Config.imagespath +  "/hourly_def.png" 
            },

            Sheet {
               id: mySheet
               content: TabbedPane {
                   id: tabFullWeather
                   showTabsOnActionBar: true 
                   Tab {
                       id: close_page
                       title: Config.tr("Back")
                       imageSource: "asset:///share/images/arrow_left.png"  
                       onTriggered: {
                            mySheet.close();
                        }
                   }
                   Tab {
                       id: day_tab
                       title: Config.tr("Day")
                       imageSource: Config.imagespath +  "/day_def.png" 
                   }
                   Tab {
                       id: night_tab 
                       title: Config.tr("Night")
                       imageSource: Config.imagespath +  "/night_def.png" 
                   }
               }
            },
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
