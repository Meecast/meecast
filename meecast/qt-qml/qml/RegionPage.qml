import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: countries 
    objectName: "regionspage"
    property int margin: 16
    property int removedStation: -1
    property string removedStationName: ""
    //Config {id: config1}
    property string source: ""
	property int source_id: -1
    property string country_name: ""

    content:  Container{
		id: absoluteLayoutContainer
		background: Color.White
		layout: AbsoluteLayout {}
		attachedObjects: [
			ComponentDefinition {
				id: nextpage3
				source: "CityPage.qml" 
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
					console.log ("Regions Size ", region_model.rowCount());
    				for (var a = 0; a < region_model.rowCount(); a++) {
						console.log("Regions ", region_model.get(a).name);
						groupDataModel.insert( {"name" : region_model.get(a).name, "number" : a, "key" : region_model.get(a).key});
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
								  preferredHeight: 40 
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
					console.log("Index in Region ", groupDataModel.data(indexPath).name);

                    city_model.populate(sourcespage.source, groupDataModel.data(indexPath).key);
					var newPage = nextpage3.createObject();
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
		    text:  Config.tr("Select region") 
			textStyle {
				base: SystemDefaults.TextStyles.TitleText
				color: Color.White
			}
		}
	}
}
