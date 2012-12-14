import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: settings
    property int margin: 16
    property int screen_width  :  768
    property int screen_height : 1280    

   

    content:  Container{
        
       id: absoluteLayoutContainer
       background: Color.White
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
                imageSource: "asset:///share/images/mask_background_grid.png"
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
			layoutProperties: AbsoluteLayoutProperties {
                                positionX: 0
                                positionY: 0
            }
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
					groupDataModel.insert( {"name" :  Config.tr("About"), "qml" : "AboutPage.qml"});
//					groupDataModel.insert( {"name" :  Config.tr("Update"), "qml" : "Update.qml"});
					groupDataModel.insert( {"name" :  Config.tr("Measurement units"), "qml" : "UnitsPage.qml"});
					groupDataModel.insert( {"name" :  Config.tr("Appearance"), "qml" : "VisualsPage.qml"});
					groupDataModel.insert( {"name" :  Config.tr("Manage locations"), "qml" : "StationsPage.qml"});
                    rootWindow.backButtonsVisible = true;
				}           
				listItemComponents: [
					 ListItemComponent {
					 	type: "header"
						Label {
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
								  horizontalAlignment:  HorizontalAlignment.Fill
								  verticalAlignment: VerticalAlignment.Center
                                  textStyle {
									  base: SystemDefaults.TextStyles.SubtitleText
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
					var newPage = nextpage.createObject();
                    rootWindow.push(newPage);
				}
			}
		} 
		Label {
			layoutProperties: AbsoluteLayoutProperties {
            	positionX: 0
                positionY: 0
            }
		    preferredWidth: 768
		    text:  Config.tr("Settings") 
			textStyle {
				base: SystemDefaults.TextStyles.TitleText
				color: Color.White
			}
		}
    }
}

