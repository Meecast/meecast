 import QtQuick 1.0
 
 Rectangle {
     id: page 
     width: 127; height: 96
     color: "black"

     property string mytext: ""
     property int texty: 0
     function temp(){
        if (Applet.temperature == "N/A" || Applet.temperature == ""){
	   if (Applet.temperature_high != "N/A"){
		  mytext = Applet.temperature_high + '°'
	          texty = 20
	   }
	   if ((Applet.temperature_low != "N/A") && (Applet.temperature_high != "N/A")){
		  mytext =  mytext + "\n"
	          texty = 0
           }
	   if (Applet.temperature_low != "N/A")
		   mytext = mytext + Applet.temperature_low + '°'
        }else{
	   texty = 20
           mytext = Applet.temperature + '°'
        }
	return mytext; 
     }
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
		 y: texty 
		 font.pointSize: 20; 
		 color: "white"
		 text: temp()
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
