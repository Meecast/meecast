import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: stations
    objectName: "stationspage"
    property int margin: 16
    property int removedStation: -1
    property string removedStationName: ""
    //Config {id: config1}


    content:  Container{
		id: absoluteLayoutContainer
		background: Color.White
		layout: AbsoluteLayout {}
		attachedObjects: [
			ComponentDefinition {
				id: nextpage
				source: "CountryPage.qml" 
			}
		]
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
                    positionY: 90
            }
			attachedObjects: [
				GroupDataModel {
					id: groupDataModel
					sortingKeys: ["number"]
                    grouping: ItemGrouping.None
				}
			]
			ListView {
				id: listview
				dataModel: groupDataModel 
				onCreationCompleted: {
    				for (var a = 0; a < source_model.rowCount(); a++) {
						console.log("SOURCE ", source_model.get(a).name);
						groupDataModel.insert( {"name" : source_model.get(a).name, "number" : a});
					}
				}           
				listItemComponents: [
					 ListItemComponent {
						 type: "item"
						 id: listitemcomp
						 Container {
							  layout: DockLayout {}
						 
							  Label {                 
								  text: ListItemData.name
								  preferredWidth: 768
								  //preferredHeight: settings.screen_height
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
					console.log("Index ", groupDataModel.data(indexPath).name);
					country_model.populate(groupDataModel.data(indexPath).name);
					var newPage = nextpage.createObject();
                    rootWindow.push(newPage, {source: groupDataModel.data(indexPath).name, source_id: groupDataModel.data(indexPath).number});
				}
			}
		}       
		Label {
			layoutProperties: AbsoluteLayoutProperties {
            	positionX: 0
                positionY: 0
            }
		    preferredWidth: 768
		    text:  Config.tr("Select the weather source") 
			textStyle {
				base: SystemDefaults.TextStyles.TitleText
				color: Color.White
			}
		}


	}
}
