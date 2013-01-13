import QtQuick 1.0
//import com.nokia.meego 1.0


 Item {
     id: container
     property alias cellColor: main_rectangle.color
     property  bool selected: false 
     signal clicked(color cellColor)

     width: 50; height: 50
     
     function unselected(){
         selected = false;
         second_rectangle.border.color = selected ? "white" : "black"
         second_rectangle.color = selected ? "white" : "black"

     }
     Rectangle {
         id: first_rectangle
         width: 50; height: 50 
         border.color: "black"
         color: "black"
         anchors.fill: parent
     }

    Rectangle {
         id: second_rectangle
         y: 1
         x: 1
         width: 48; height: 48 
         border.color: selected ? "white" : "black"
         color: selected ? "white" : "black"
    }
    Rectangle {
         id: thrid_rectangle
         y: 2
         x: 2
         width: 46; height: 46 
         border.color: "black"
         color: "black"
     }

     Rectangle {
         id: main_rectangle
         width: 44; height: 44
         y: 3
         x: 3
         border.color: "white"
     }

     MouseArea {
         anchors.fill: parent
         onClicked: { container.clicked(container.cellColor); second_rectangle.border.color = "white" }
         
     }
 }
