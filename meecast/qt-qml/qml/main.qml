import bb.cascades 1.0


NavigationPane {
    
    id: rootWindow
    property int main_icon_size : 240
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

    // Create the initial screen
    Page {
        id: main
        property int screen_width : 1280
        property int screen_height : 768
        property int day: 0
        property bool current: false
        property bool current_value: false
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

        function update_list(){
            forrecasts_list.dataModel.clear();
            if (Current.getdata(0, "current"))
                current_value = Current.getdata(0, "current");
            else
                current_value = false;
            console.log("Forecast_model.rowCount() ",  Forecast_model.rowCount());
            console.log("Cuurent.rowCount() ",  Current.rowCount());
            for (var a = 0; a < Forecast_model.rowCount() + 1; a++) {
                if (a == 0){
                    if (Current.rowCount() > 0){
                        fulldate = Current.getdata(a, "fulldate");
                        shortdate = Current.getdata(a, "shortdate");
                        temp_high = Current.getdata(a, "temp_high");
                        temp_low  = Current.getdata(a, "temp_low");
                        hi_temp_color =  main.getColor(Current.getdata(a, "temp_high"));
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
                    hi_temp_color =  main.getColor(Forecast_model.getdata(a-1, "temp_high"));
                    pict = Config.iconspath + "/" + Config.iconset + "/" + Forecast_model.getdata(a-1, "pict");
                    bg_color = ((a-1) % 2 != 0) ? "#0f0f0f" : "#000000";
                    title_text = Forecast_model.getdata(a-1, "description");
                    wind_direction = Forecast_model.getdata(a-1, "wind_direction");
                    wind_speed = Forecast_model.getdata(a-1, "wind_speed");
                    humidity = Forecast_model.getdata(a-1, "humidity");
                    pressure = Forecast_model.getdata(a-1, "pressure");
                    background_image = "";
                }
                if (a == 1){
                    bg_color = current_rect_back_background;
                    background_image = "asset:///share/images/row_background.png"
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
                         "background_image": background_image,
                        }
                    )

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
                   current_temp_text = current_temp_text + '/'+ Current.getdata(0, "temp_low") + '°'
                current_rect_back_background = main.getColor(Current.getdata(0, "temp_high"));
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
                        preferredWidth: 768
                        preferredHeight: 1000
                    }

                    Container{
                        id: startview
                        visible : Config.stationname == "Unknown" ? true : false;
                        preferredWidth: 768
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
                            Qt.main_icon_size = main_icon_size
                            Qt.row_icon_size = row_icon_size
                            main.update_list();
                        }

                        listItemComponents: [
                            ListItemComponent {
                                type: "item"
                                Container{
                                    Container{
                                    layout: AbsoluteLayout {}
//                                    visible: ListItemData.number == 0 && ListItemData.current_value == true ? true : false;
                                    visible: ListItemData.number == 0 ? true : false;

                                    Container{
                                        preferredWidth: 768
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
                                            preferredWidth: 768
                                            preferredHeight: 420 
                                            layoutProperties: AbsoluteLayoutProperties {
                                                positionY: 0
                                            }
                                            background: Color.create(ListItemData.bg_color)
                                        }
                                        ImageView {
                                            imageSource: ListItemData.background_image
                                            visible: ListItemData.fulldate != "" ? true :false
                                            preferredWidth: 768
                                            preferredHeight: 420 
                                            layoutProperties: AbsoluteLayoutProperties {
                                                positionY: 0
                                            }
                                        }
                         
                                        Container{
                                            preferredWidth: 768
                                            visible: ListItemData.fulldate != "" ? true :false
                                            id: current_temp_and_other_paremeters
                                            layout: DockLayout {}
                                            layoutProperties: AbsoluteLayoutProperties {
                                                positionY: 190.0
                                            }
              
                                            Container{
                                                id: current_temperature
                                                horizontalAlignment: HorizontalAlignment.Left
                                                preferredWidth: ListItemData.current_temp_text.length < 5 ? 768/2 - Qt.main_icon_size/2 : 768/2 - Qt.main_icon_size/2 + 50
                                                verticalAlignment: VerticalAlignment.Center
                                                Container{
                                                   Label {
                                                        id: current_temp_text_id
                                                        text: ListItemData.current_temp_text
                                                        preferredWidth: ListItemData.current_temp_text.length < 5 ? 768/2 - Qt.main_icon_size/2 : 768/2 - Qt.main_icon_size/2 + 50
                                                        horizontalAlignment: HorizontalAlignment.Center
                                                        verticalAlignment: VerticalAlignment.Center
                                                        textStyle.textAlign: TextAlign.Center
                                                        textStyle {
                                                            fontSize: FontSize.PointValue
                                                            fontWeight: FontWeight.W100
                                                            fontSizeValue: ListItemData.current_temp_text.length < 5 ? 18.0 : 14 
                                                            color: Color.White
                                                        }
                                                    }
                                                }
                                            }
                                            Container{
                                                id: humidity_wind_id 
                                                horizontalAlignment: HorizontalAlignment.Right
                                                preferredWidth: 768/2 - Qt.main_icon_size/2                   
                                                verticalAlignment: VerticalAlignment.Center
                                                Container{
                                                    layout: AbsoluteLayout {} 
                                                    horizontalAlignment: HorizontalAlignment.Center
                                                    Container{
                                                        id: humidity
                                                        horizontalAlignment: HorizontalAlignment.Left
                                                        visible: ListItemData.humidity == "N/A" ? false : true; 
                                                        layoutProperties: AbsoluteLayoutProperties {
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
                                                                fontSizeValue: 6.0
                                                                color: Color.White
                                                            }

                                                        }
                                                    }
                                                    Container{
                                                        id: wind_direction
                                                        layoutProperties: AbsoluteLayoutProperties {
                                                            positionY: 50.0
                                                        }

                                                        visible: ListItemData.wind_direction == Qt.Config.tr("N/A") ? false : true; 
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
                                                                fontSizeValue: 6.0
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
                                                            positionY: 100.0
                                                        }

                                                        layout: AbsoluteLayout {
                                                        }
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
                                                                fontSizeValue: 6.0
                                                                color: Color.White
                                                            }
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
                                                positionY: 350.0
                                            }
                                            Label {                 
                                                id: title_text
                                                text: ListItemData.description_text 
                                                horizontalAlignment: HorizontalAlignment.Center
                                                textStyle {
                                                    fontSize: FontSize.PointValue
                                                    fontSizeValue: 6.0
                                                    fontWeight: FontWeight.W100
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
                                                    fontSizeValue: 10.0
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
                                            layoutProperties: AbsoluteLayoutProperties {
                                                positionX: 768/2 - Qt.main_icon_size/2 
                                                positionY: 145
                                            }
                                            preferredWidth: Qt.main_icon_size 
                                            preferredHeight: Qt.main_icon_size 
                                            imageSource: ListItemData.pict 
                                            horizontalAlignment: HorizontalAlignment.Center
                                            overlapTouchPolicy: OverlapTouchPolicy.Allow
                                            touchPropagationMode: TouchPropagationMode.PassThrough
                                        } 
                                        Container{
                                            horizontalAlignment: HorizontalAlignment.Left
                                            preferredWidth: 768 
                                            layoutProperties: AbsoluteLayoutProperties {
                                                positionY: 120.0
                                            }
                                            Label {
                                                visible: ListItemData.fulldate != "" ? true :false
                                                text: ListItemData.current == true ? Qt.Config.tr("Now") : Qt.Config.tr("Today")
                                                horizontalAlignment: HorizontalAlignment.Center
                                                verticalAlignment: VerticalAlignment.Center
                                                textStyle.textAlign: TextAlign.Center
                                                textStyle {
                                                    fontSize: FontSize.PointValue
                                                    fontWeight: FontWeight.W100
                                                    fontSizeValue: 5.0
                                                    color: Color.Gray
                                                }
                                            }
                                        }
                                    }
                                    Container{
                                        visible: ListItemData.number == 0 ? false : true;
                                        background: Color.create(ListItemData.bg_color)
                                        preferredWidth: 768
                                        preferredHeight: 120 
                                        Container{
                                            visible: ListItemData.background_image != "" ? true : false;
                                            ImageView {
                                                 imageSource: ListItemData.background_image
                                                 horizontalAlignment: HorizontalAlignment.Center  
                                            }
                                        }
                                        layout: AbsoluteLayout {
                                        }
                                        Container{
                                            layout: DockLayout {
                                            }
                                            preferredWidth: 768
                                            preferredHeight: 120 
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
                                                        preferredWidth: 768/2 - Qt.row_icon_size/2 - 20
                                                        textStyle {    
                                                            base: SystemDefaults.TextStyles.BodyText 
                                                            fontWeight: FontWeight.W100
                                                            color: Color.Gray
                                                        }
                                                    }
                                                    Label {
                                                        text: ListItemData.shortdate
                                                        preferredWidth: 768/2 - Qt.row_icon_size/2 - 20
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
                                console.log("Index ", (groupDataModel.data(indexPath).numberi - 1));
                                main.day = groupDataModel.data(indexPath).number - 1;
                                main.current = false;
                            }
                            main.day_period = "day";
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
