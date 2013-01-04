import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: settings2
    property int margin: 16
    property int screen_width  :  768
    property int screen_height : 1280    

   

    content:  Container{
        
       function update_list(){
                groupDataModel.clear();
groupDataModel.insert( {"group": Config.tr("Apperance") ,"name" :  Config.tr("Icons"), "qml" : "IconsetPage.qml", "status" : Config.iconset });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Visibility"), "qml" : "VisibilityPage.qml", "status" : Config.visibleunit });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Pressure"), "qml" : "PressurePage.qml", "status" : Config.pressureunit });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Wind speed"), "qml" : "WindSpeedPage.qml", "status" : Config.windspeedunit });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Temperature"), "qml" : "TemperaturePage.qml", "status" : Config.temperatureunit });

       }
       id: settings2_absoluteLayoutContainer
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
                    sortingKeys: ["group"]
                    grouping: ItemGrouping.ByFullValue 
				},
				ComponentDefinition {
					id: nextpage
				}
			]
			ListView {
				id: listview
				dataModel: groupDataModel
				onCreationCompleted: {
					groupDataModel.insert( {"group": Config.tr("Apperance") ,"name" :  Config.tr("Icons"), "qml" : "IconsetPage.qml", "status" : Config.iconset });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Visibility"), "qml" : "VisibilityPage.qml", "status" : Config.visibleunit });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Pressure"), "qml" : "PressurePage.qml", "status" : Config.pressureunit });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Wind speed"), "qml" : "WindSpeedPage.qml", "status" : Config.windspeedunit });
					groupDataModel.insert( {"group": Config.tr("Measurement units") ,"name" :  Config.tr("Temperature"), "qml" : "TemperaturePage.qml", "status" : Config.temperatureunit });
                    rootWindow.backButtonsVisible = true;
				}           
				listItemComponents: [
			        ListItemComponent {
						 type: "item"
						 id: listitemcomp
                             StandardListItem {
                                title: ListItemData.name
                                status: ListItemData.status
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

