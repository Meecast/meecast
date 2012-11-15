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
        
       id: absoluteLayoutContainer
       background: Color.Black
       layout: AbsoluteLayout {}
       Container{
                layoutProperties: AbsoluteLayoutProperties {
                	positionX: 0
                    positionY: 0
                }
                background: Color.Black
                preferredWidth: 768
                preferredHeight: 90
        }                       
       ImageView {
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 90
                 }
                imageSource: "asset:///images/mask_background_grid.png"
                preferredWidth: 768  
        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0
                                positionY: 350
                }
                background: Color.Black
                preferredWidth: 768
                preferredHeight: 1000
        }
        Container{
			attachedObjects: [
				GroupDataModel {
					id: groupDataModel
				},
				ComponentDefinition {
					id: nextpage
				}
			]
			ListView {
				id: listview
				dataModel: groupDataModel
				onCreationCompleted: {
					groupDataModel.insert( {"name" : "Update", "qml" : "Update.qml"});
					groupDataModel.insert( {"name" : "Measurement units", "qml" : "UnitsPage.qml"});
					groupDataModel.insert( {"name" : "Appearance", "qml" : "VisualsPage.qml"});
					groupDataModel.insert( {"name" : "Manage locations", "qml" : "StationsPage.qml"});
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
						 id: listitemcomp
						 Container {
							  layout: DockLayout {}
						 
							  Label {                 
								  text: ListItemData.name
								  preferredWidth: 768
								  preferredHeight: settings.screen_height
								  horizontalAlignment:  HorizontalAlignment.Fill
								  verticalAlignment: VerticalAlignment.Center
                                  textStyle {
									  base: SystemDefaults.TextStyles.TitleText
									  color: Color.White
								  }
							  }
							  Label {                 
								text: ">"
								horizontalAlignment: HorizontalAlignment.Right
								verticalAlignment: VerticalAlignment.Center
								textStyle {
									base: SystemDefaults.TextStyles.BigText
									color: Color.White
								}
							  }
						 }
					}
				]
				onTriggered: {             
					console.log("Index ", groupDataModel.data(indexPath).qml);
					nextpage.source = groupDataModel.data(indexPath).qml;
					navigationPane.push(nextpage.createObject())
				}
			}
		}       
    }
}

