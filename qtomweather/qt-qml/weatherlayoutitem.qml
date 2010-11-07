import Qt 4.7

LayoutItem {    //Sized by the layout
    id: weatherlayoutitem

    minimumSize: "32x32"
    maximumSize: "300x300"
    preferredSize: "100x100"

    Rectangle { color: "yellow"; anchors.fill: parent
        Image {
             id: forecast_icon
             source: Config.iconspath + "/" + Config.iconset+"/" + Forecast.icon + ".png"
             anchors.centerIn: parent
         }
    Text {		
        id: temperature
    //    text: Forecast.temperature
        text: Forecast.temperature
//	text: "ddddd" 
//	font.pointSize: 12
//	wrap: true
//	color: "white"
	
	//ancors
//	anchors.top: title.bottom
//	anchors.topMargin: 5
//	anchors.right: rect.right
//	anchors.rightMargin: 5
//	anchors.left: avatar.right
//	anchors.leftMargin : 5

//	onTextChanged: rect.height = calculateMyHeight();
    }

    }

    //Rectangle {
    //   width: 100; height: 100
    //   anchors.top: parent.top; anchors.right: parent.right
    //   color: "green"
    //}

}
