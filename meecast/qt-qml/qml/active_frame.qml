import bb.cascades 1.0

Container {
    property string current_temp_text: ""
    property color current_rect_back_background :  "Red"
    property string main_icon_imageSource : ""
    property string title_text_text : ""
    property string humidity_text: ""
    property string pressure_text: ""
    function onConfigChanged() {
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
                console.log("update_current_data!!!!!!!!!!!!!");
                //left_arrow = Config.prevstationname == "" ? false : true;
                //right_arrow = Config.nextstationname == "" ? false : true;
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
    
    
    id: active_main
    objectName: "mainContainer"
    background:  Color.create(current_rect_back_background)
    onCreationCompleted: {
        Config.configChanged.connect (active_main.onConfigChanged);
        active_main.update_current_data();
        console.log("Active created");
    }
Container {  
    layout: AbsoluteLayout {}  
    ImageView {
        imageSource: "asset:///share/images/mask_background.png"
            scalingMethod: ScalingMethod.AspectFill
            preferredHeight: 396
            preferredWidth: 334
        }
    ImageView {
        imageSource: main_icon_imageSource
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 100
            positionY: 100
        }
        //verticalAlignment: VerticalAlignment.Center
        //horizontalAlignment: HorizontalAlignment.Center
        preferredHeight: 256
        preferredWidth: 256
    }
    Label {
        objectName: "temperature"
        text: current_temp_text
        multiline: true
        verticalAlignment: VerticalAlignment.Top
        horizontalAlignment: HorizontalAlignment.Left
        textStyle.fontSize: FontSize.XSmall
    }
    Label {
        text: title_text_text
        layoutProperties: AbsoluteLayoutProperties {
            positionX: 0
            positionY: 300
        }  
        multiline: true
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Bottom
        textStyle.fontSize: FontSize.Small
    }
}
}
