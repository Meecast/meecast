import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: settings
    property int margin: 16
    property int screen_height : Config.get_height_resolution() 
    property int screen_width : Config.get_width_resolution() 
   

    content:  Container{
        
       id: absoluteLayoutContainer
       background: Color.create("#262626")
//       background: Color.White
       layout: AbsoluteLayout {}
       Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            layout: DockLayout {}
            background: Color.Black
            preferredWidth: screen_width
            preferredHeight: 120 
            Label {
                verticalAlignment: VerticalAlignment.Center
                preferredWidth: screen_width
                preferredHeight: 120 
                text:  Config.tr("Settings") 
                textStyle {
                    base: SystemDefaults.TextStyles.TitleText
                    color: Color.White
                }
            }
       }                       
//       ImageView {
//                layoutProperties: AbsoluteLayoutProperties {
//                    positionX: 0
//                    positionY: 90
//                 }
//                imageSource: "asset:///share/images/mask_background_grid.png"
//                preferredWidth: screen_width  
//        }
        Container{
                layoutProperties: AbsoluteLayoutProperties {
                    positionX: 0
                    positionY: 350
                }
                background: Color.create("#262626")
                preferredWidth: screen_width
                preferredHeight: screen_height - 280
        }
		Container{
			layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 120 
            }
			attachedObjects: [
				GroupDataModel {
					id: groupDataModel
                    grouping: ItemGrouping.None
				},
				ComponentDefinition {
					id: nextpage
				}
			]
			ListView {
				id: listview
				dataModel: groupDataModel
                
				onCreationCompleted: {
					groupDataModel.insert( {"name" :  Config.tr("About"), "qml" : "AboutPage.qml"});
//					groupDataModel.insert( {"name" :  Config.tr("Update"), "qml" : "Update.qml"});
			//		groupDataModel.insert( {"name" :  Config.tr("Measurement units"), "qml" : "UnitsPage.qml"});
			//		groupDataModel.insert( {"name" :  Config.tr("Appearance"), "qml" : "VisualsPage.qml"});
					groupDataModel.insert( {"name" :  Config.tr("Settings"), "qml" : "SettingsPage2.qml"});
					groupDataModel.insert( {"name" :  Config.tr("Manage locations"), "qml" : "StationsPage.qml"});
                    rootWindow.backButtonsVisible = true;
                    Qt.screen_width = screen_width  
                    Qt.screen_height = screen_height
				}           
				listItemComponents: [
					 ListItemComponent {
					 	type: "header"
						Label {
						  text: "" 
		                }
					 },   
					 ListItemComponent {
						 type: "item"
						 id: listitemcomp
						 Container {
							  layout: DockLayout {}
                              preferredHeight: 120
                              background:  Color.create("#262626") 
							  Label {                 
								  text: ListItemData.name
								  preferredWidth: Qt.screen_width
								  horizontalAlignment:  HorizontalAlignment.Fill
								  verticalAlignment: VerticalAlignment.Center
                                  textStyle {
									  base: SystemDefaults.TextStyles.TitleText
									  color: Color.White
                                      fontWeight: FontWeight.W100
								  }
							  }
                              ImageView {
                                  imageSource: "asset:///share/images/arrow_right.png"
                                  horizontalAlignment: HorizontalAlignment.Right
								  verticalAlignment: VerticalAlignment.Center
                              }
					     }
					}
				]
				onTriggered: {             
					console.log("Index ", groupDataModel.data(indexPath).qml);
					nextpage.source = groupDataModel.data(indexPath).qml;
					var newPage = nextpage.createObject();
                    rootWindow.push(newPage);
				}
			}
		} 
    }
}

