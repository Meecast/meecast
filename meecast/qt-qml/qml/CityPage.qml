import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: cities 
    objectName: "citiesrpage"

    content:  Container{
        id: absoluteLayoutContainer
        background: Color.White
        layout: AbsoluteLayout {}
        attachedObjects: [
            ComponentDefinition {
                id: nextpage4
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
                    console.log ("Stations Size ", city_model.rowCount());
                    for (var a = 0; a < city_model.rowCount(); a++) {
                        console.log("Stations ", city_model.get(a).name);
                        groupDataModel.insert( {"name" : city_model.get(a).name, "number" : a, "key" : city_model.get(a).key});
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
                         }
                    }
                ]
                onTriggered: {             
                    console.log("Index in Station ", groupDataModel.data(indexPath).name);
                    Config.saveStation1(groupDataModel.data(indexPath).key, groupDataModel.data(indexPath).name, regionspage.region, countries.country, sourcespage.source, sourcespage.key);
                    rootWindow.navigateTo(main);
                }
            }
        }       
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredWidth: 768
            text:  Config.tr("Select location") 
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.White
            }
        }
    }
}
