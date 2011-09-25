 import QtQuick 1.0

 Rectangle {
     id: page 
     width: 127; height: 96
     color: "black"

     MouseArea {
         anchors.fill: parent
          onClicked: {
	 	Applet.startapplication() 
	  }
     }


     Text {
         id: station 
         text: Applet.station 
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
		 text: Applet.temperature 
		 y: 20 
		 font.pointSize: 20; 
		 color: "white"
	     }

	     Image {
		 id: icon
		 source: Applet.icon 
		 anchors.left: temperature.right 
		 width:  72 
		 height: 72
		 smooth: true	
	     }
     }

 }
