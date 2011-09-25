 import QtQuick 1.0

 Rectangle {
     id: page 
     width: 127; height: 96
     color: "black"

     Text {
         id: station 
         text: "London"
         y: 2 
         anchors.horizontalCenter: page.horizontalCenter
         font.pointSize: 12; 
	 color: "white"
     }
     Rectangle{
             id: submain
             color: "black"
	     anchors.top: station.bottom
  	     width:  72
	     Text {
		 id: temperature 
		 text: "+10"
		 y: 20 
		 font.pointSize: 20; 
		 color: "white"
	     }

	     Image {
		 id: icon
		 source: "/opt/com.meecast.omweather/share/icons/Meecast/28.png" 
		 anchors.left: temperature.right 
		 width:  72 
		 height: 72
		 smooth: true	
	     }
     }

 }
