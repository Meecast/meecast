import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: sourcespage
    objectName: "sourcespage"
    property int margin: 16
    property int removedStation: -1
    property string removedStationName: ""
    //Config {id: config1}
    property string source : ""
    property string key : ""
    property int screen_height : Config.get_height_resolution() 
    property int screen_width : Config.get_width_resolution() 
    property int menu_height : Config.get_height_resolution() == 1280 ? 138 : 110;

    content:  Container{
        id: absoluteLayoutContainer
        background: Color.create("#262626")
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
            layout: DockLayout {}
            preferredWidth: screen_width 
            preferredHeight: 90
            Label {
                preferredWidth: screen_width 
                verticalAlignment: VerticalAlignment.Center
                text:  Config.tr("Select the weather source") 
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                }
            }
        } 
//        ImageView {
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
                preferredHeight: 80 
        }
        Container{
            preferredHeight: screen_height - 90 - menu_height 
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
                        groupDataModel.insert( {"name" : source_model.get(a).name, "number" : a, "key" : source_model.get(a).key});
                    }
                    Qt.screen_width = screen_width  
                    Qt.screen_height = screen_height
                }           
                listItemComponents: [
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
                                  //preferredHeight: settings.screen_height
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
                    console.log("Index ", groupDataModel.data(indexPath).name);
                    country_model.populate(groupDataModel.data(indexPath).name);
                    var newPage = nextpage.createObject();
                    sourcespage.source = groupDataModel.data(indexPath).name;
                    sourcespage.key = groupDataModel.data(indexPath).key;
                    rootWindow.push(newPage, {source: groupDataModel.data(indexPath).name, source_id: groupDataModel.data(indexPath).number});
                }
            }
        }       
    }
}
