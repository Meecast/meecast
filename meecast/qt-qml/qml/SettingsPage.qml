import QtQuick 1.0
import bb.cascades 1.0

Page {
    
    id: settings
    property int margin: 16
    

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
      background: Color.Black
      Container {
            id: title_rect
            Label {
                id: title
                text: "Settings"
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                // Apply a text style to create large, light gray text
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    color: Color.White
                }
            }
      }     
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
                     type: "item"
                     Container {
                          Label {                 
                              text: ListItemData.name
                              // Apply a text style to create large, light gray text
                              textStyle {
                                  base: SystemDefaults.TextStyles.SmallText
                                  color: Color.White
                              }
                          }
                          ImageView {
                          //source: "image://theme/icon-m-common-drilldown-arrow-inverse"
                         //anchors.right: parent.right
                         //anchors.verticalCenter: parent.verticalCenter
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

