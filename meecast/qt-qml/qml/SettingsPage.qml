import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: settings
    property int margin: 16
    property int screen_width  :  768
    property int screen_height : 1280    

    function openFile(file)
    {
        var component = Qt.createComponent(file);
        if (component.status == Component.Ready){
            pageStack.push(component);
        }else {
            console.log("error open file "+file);
        }
    }
    

    content:  Container{
        
      id: cont
      //background: Color.Black
      background: paint.imagePaint
      attachedObjects: [
                      ImagePaintDefinition {
                          id: paint
                         // imageSource: "asset:///images/mybackground.png"
                          //repeatPattern: RepeatPattern.XY
                      }
      ]    
      Container{
        attachedObjects: [
                    GroupDataModel {
                        id: groupDataModel
                    }
        ]

        ListView {
            id: listview
            dataModel: groupDataModel
            onCreationCompleted: {
                groupDataModel.insert( {"name" : "Manage locations", "qml" : "StationsPage.qml"});
                groupDataModel.insert( {"name" : "Measurement units", "qml" : "UnitsPage.qml"});
                groupDataModel.insert( {"name" : "Appearance", "qml" : "VisualsPage.qml"});
                groupDataModel.insert( {"name" : "Update", "qml" : "Update.qml"});
            }           
            listItemComponents: [
                 ListItemComponent {
                         type: "header"
                         Label {
                              text: "Settings"
                              textStyle {
                                  base: SystemDefaults.TextStyles.BigText
                                  color: Color.White
                              }
                         }
                 
                 },   
                 ListItemComponent {
                     type: "item"
                     id: listitemc
                     Container {
                          //preferredWidth: 768
                          layout: DockLayout {
                          }
                     
                          Label {                 
                              text: ListItemData.name
                              preferredWidth: 768
                              preferredHeight: screen_height
                              horizontalAlignment:  HorizontalAlignment.Fill
                              verticalAlignment: VerticalAlignment.Center
                              // Apply a text style to create large, light gray text
                              textStyle {
                                  base: SystemDefaults.TextStyles.TitleText
                                  color: Color.White
                              }
                          }
                          Label {                 
                               text: ">"
                               horizontalAlignment: HorizontalAlignment.Right
                               verticalAlignment: VerticalAlignment.Center
                               // Apply a text style to create large, light gray text
                                textStyle {
                                   base: SystemDefaults.TextStyles.BigText
                                   color: Color.White
                                 }
                          }
                          
                     }
                }
            ]
            onTriggered: {
                      
                        settings.openFile(model.page);
            
                }
            }
        }
        
       
    }
    
}

