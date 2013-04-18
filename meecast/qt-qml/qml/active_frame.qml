import bb.cascades 1.0

Container {
    property string current_temp_text: ""
    property color current_rect_back_background :  "White"
    property string main_icon_imageSource : ""
    property string title_text_text : ""
    property string humidity_text: ""
    property string pressure_text: ""
    property string wind_speed_text: ""
    property string fulldate: ""
    property bool current_value: false
    property int wind_direction_angle : 0
    property string wind_direction_text: ""
    property string stationname_text: ""

    property int screen_height : Config.get_height_resolution() >= 1280 ? 396 : 211   
    property int screen_width : Config.get_width_resolution() >= 768 ? 334 : 310
//    property int menu_height : Config.get_height_resolution() == 1280 ? 138 : 110;

   
    function onConfigChanged() {
        update_current_data()
        console.log("OnConfigChanged!!!!!");
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
    
     function update_current_data(){
                current_weatherview.visible = (Config.stationname != "Unknown" && Current.rowCount() != 0)  ? true : false;
                startview.visible = Config.stationname == "Unknown" ? true : false;
                notrefreshview.visible = (Config.stationname != "Unknown" &&  Current.rowCount() == 0) ? true : false            
                stationname_text = Config.stationname == "Unknown" ? "MeeCast" : Config.stationname
                                
                console.log("update_current_data!!!!!!!!!!!!!");
                if (Current.getdata(0, "temp") == "N/A"){
                    current_temp_text = ""
                    if (Current.getdata(0, "temp_high") != "N/A")
                       current_temp_text = Current.getdata(0, "temp_high") + '°'
                    if ((Current.getdata(0, "temp_low") != "N/A") && (Current.getdata(0, "temp_high") != "N/A"))
                    if (Current.getdata(0, "temp_low") != "N/A")
                       current_temp_text = current_temp_text + '/'+ Current.getdata(0, "temp_low") + '°'
                  //     console.log("Color!!!! ", Current.getdata(0, "temp_high"));
                    if (Current.getdata(0, "temp_high") != "N/A"){
                        current_rect_back_background = active_main.getColor(Current.getdata(0, "temp_high"));
                    }else{
                    //   console.log("Color!!!! ", Current.getdata(0, "temp_low"));
                        if (Current.getdata(0, "temp_low") != "N/A"){
                            current_rect_back_background = active_main.getColor(Current.getdata(0, "temp_high"));
                        }else{
                            current_rect_back_background = "#000000";
                        }
                    }
                }else{
                   current_temp_text = Current.getdata(0, "temp") + '°'
                   console.log(Current.getdata(0, "temp"));
                   if (Current.getdata(0, "temp") > -273){
                       current_rect_back_background = active_main.getColor(Current.getdata(0, "temp"));
                   }
                   
                }
                main_icon_imageSource = Config.iconspath + "/" + Config.iconset + "/" + Current.getdata(0, "pict")
                title_text_text = Config.tr(Current.getdata(0, "description"));
                fulldate = Current.getdata(0, "fulldate");
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
                    wind_direction_angle =  active_main.getAngle(Current.getdata(0, "wind_direction"));
                }else
                    wind_direction_angle = 0;
                wind_direction_text = Config.tr(Current.getdata(0, "wind_direction"));
                current_value = Current.getdata(0, "current");
            }
    
    
    id: active_main
    objectName: "mainContainer"
    background:  Color.create(current_rect_back_background)
    onCreationCompleted: {
        Config.configChanged.connect (active_main.onConfigChanged);
        ActiveFrame.CurrentChanged.connect(active_main.update_current_data);
        active_main.update_current_data();
        console.log("Active created");
    }
    Container {
        id: startview
        layout: AbsoluteLayout {}
        visible : Config.stationname == "Unknown" ? true : false;
        ImageView {
            imageSource: "asset:///share/images/mask_background.png"
            scalingMethod: ScalingMethod.AspectFill
            preferredHeight: screen_height
            preferredWidth: screen_width
        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                }
                preferredWidth: screen_width
                Label {
                    text: stationname_text  
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom
                    textStyle.textAlign: TextAlign.Center
                    textStyle {
                        fontSize: FontSize.PointValue
                        //fontWeight: FontWeight.W100
                        fontSizeValue: 7 
                        color: Color.White
                    }   
                    preferredWidth: screen_width
                }
        }
        Label {
            text: Config.tr("No locations are set up yet.") 
            horizontalAlignment: HorizontalAlignment.Center
            multiline: true
            textStyle.textAlign: TextAlign.Center
            textStyle {
                fontSize: FontSize.PointValue
                //fontWeight: FontWeight.W100
                fontSizeValue: 8
                color: Color.create("#999999")
            }
            layoutProperties: AbsoluteLayoutProperties {
                positionY: screen_height/2 - 40 
            }
        }
        
    }
    Container {
        id: notrefreshview
        layout: AbsoluteLayout {}
        visible : (  Config.stationname != "Unknown" &&  Current.rowCount() == 0) ? true : false
        ImageView {
            imageSource: "asset:///share/images/mask_background.png"
            scalingMethod: ScalingMethod.AspectFill
            preferredHeight: screen_height
            preferredWidth: screen_width
        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                }
                preferredWidth: screen_width 
                Label {
                    text: stationname_text  
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom
                    textStyle.textAlign: TextAlign.Center
                    textStyle {
                        fontSize: FontSize.PointValue
                        //fontWeight: FontWeight.W100
                        fontSizeValue: 7 
                        color: Color.White
                    }   
                    preferredWidth: screen_width 
                }
        }
        Label {
            text: Config.tr("Looks like there's no info for this location.")
            horizontalAlignment: HorizontalAlignment.Center
            multiline: true
            textStyle.textAlign: TextAlign.Center
            textStyle {
                fontSize: FontSize.PointValue
                //fontWeight: FontWeight.W100
                fontSizeValue: 8
                color: Color.create("#999999")
            }
            layoutProperties: AbsoluteLayoutProperties {
                positionY: screen_height/2 - 40  
            }
        }
    }
    Container {
        id: current_weatherview  
        layout: AbsoluteLayout {}  
        ImageView {
            imageSource: "asset:///share/images/mask_background.png"
                scalingMethod: ScalingMethod.AspectFill
                preferredHeight: screen_height
                preferredWidth: screen_width
        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 0
                }
                preferredWidth: screen_width
                Label {
                    text: stationname_text  
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom
                    textStyle.textAlign: TextAlign.Center
                    textStyle {
                        fontSize: FontSize.PointValue
                        //fontWeight: FontWeight.W100
                        fontSizeValue: 7 
                        color: Color.White
                    }   
                    preferredWidth: screen_width  
                }
        }
        ImageView {
            imageSource: main_icon_imageSource
            layoutProperties: AbsoluteLayoutProperties {
                positionY: Config.get_height_resolution() >= 1280 ?  0.3 * screen_height : 0.1 * screen_height
            }
            preferredHeight: Config.get_height_resolution() >= 1280 ? 256 : 200
            preferredWidth: Config.get_height_resolution() >= 1280 ? 256 : 200
        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: Config.get_height_resolution() >= 1280 ?  0.176 * screen_height : 0.08 * screen_height
                }
                preferredWidth: screen_width 
            Label {
                text: current_value == true ? Config.tr("Now") : Config.tr("Today")
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Bottom
                textStyle.fontSize: FontSize.Small
                preferredWidth: screen_width 
            }
        }
        Container{
            preferredWidth: screen_width 
            layoutProperties: AbsoluteLayoutProperties {
                positionY: Config.get_height_resolution() >= 1280 ?  20 : 20
            }        
            Label {
                objectName: "temperature"
                text: current_temp_text.length < 5 ? current_temp_text : current_temp_text.replace("/","\n") 
                multiline: true
                verticalAlignment: VerticalAlignment.Top
                horizontalAlignment: HorizontalAlignment.Right
                textStyle.textAlign: TextAlign.Right
                textStyle {
                    fontSize: FontSize.PointValue
                    fontWeight: FontWeight.W100
                    fontSizeValue: Config.get_height_resolution() >= 1280 ? (current_temp_text.length < 5 ? 27.0 : 20)  : (current_temp_text.length < 5 ? 20.0 : 13) 
                    //fontSizeValue: current_temp_text.length < 5 ? 27.0 : 20
                    color: Color.White
                }
            }
        }
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: Config.get_height_resolution() >= 1280 ?  0.8 * screen_height : 0.7 * screen_height
            }
            layout: DockLayout {}
            preferredWidth: screen_width 
            preferredHeight: 85
            Container{
                verticalAlignment: VerticalAlignment.Center
                Label {
                    text: title_text_text  
                    multiline: true
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.textAlign: TextAlign.Center
                    preferredWidth: screen_width 
                    textStyle {
                        fontSize: FontSize.PointValue
                        fontSizeValue: Config.get_height_resolution() >= 1280 ?  7 : 6
                        color: Color.White 
                    }
                }
            }
        }
    }
}
