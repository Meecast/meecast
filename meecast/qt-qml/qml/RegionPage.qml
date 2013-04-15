import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: regionspage 
    objectName: "regionspage"
    property int margin: 16
    property int removedStation: -1
    property string removedStationName: ""
    property string first_string: ""
    property string second_string: ""
    property string region: ""
    property int screen_height : Config.get_height_resolution() 
    property int screen_width : Config.get_width_resolution() 
    property int menu_height : Config.get_height_resolution() == 1280 ? 138 : 110;


    content:  Container{
        id: absoluteLayoutContainer
        background: Color.create("#262626")
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
            layout: DockLayout {}
            preferredWidth: 768
            preferredHeight: 120
            Label {
                verticalAlignment: VerticalAlignment.Center
                preferredWidth: 768
                text:  Config.tr("Select region") 
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    color: Color.White
                }
            }
        } 
//        ImageView {
//            layoutProperties: AbsoluteLayoutProperties {
//                positionX: 0
//                positionY: 90
//             }
//            imageSource: "asset:///share/images/mask_background_grid.png"
//            preferredWidth: 768  
//        }
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 350
            }
            background: Color.create("#262626")
            preferredWidth: 768
            preferredHeight: 1000
        }
        TextField {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 120
            }
            hintText: Config.tr ("Filter by...")
            onTextChanging: { 
                groupDataModel.clear();
                if (text != ""){
                    for (var a = 0; a < region_model.rowCount(); a++) {
                        first_string = text.toLocaleUpperCase()
                        second_string =  region_model.get(a).name.toLocaleUpperCase();
                        if (second_string.indexOf(first_string) >= 0){
                            groupDataModel.insert( {"name" : region_model.get(a).name, "number" : a, "key" : region_model.get(a).key });
                        }
                    }
                }else{
                    for (var a = 0; a < region_model.rowCount(); a++) {
                        groupDataModel.insert( {"name" : region_model.get(a).name, "number" : a, "key" : region_model.get(a).key });
                    }
                }
            }
        }

        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 200
            }
            preferredHeight: screen_height - 200 - menu_height 
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
                    for (var a = 0; a < region_model.rowCount(); a++) {
                        groupDataModel.insert( {"name" : region_model.get(a).name, "number" : a, "key" : region_model.get(a).key});
                    }
                }           
                listItemComponents: [
                     ListItemComponent {
                         type: "item"
                         id: listitemcomp
                         Container {
                              background:  Color.create("#262626") 
                              layout: DockLayout {}
                              preferredHeight: 120
                              Label {                 
                                  text: ListItemData.name
                                  preferredWidth: 768
                                  preferredHeight: 40 
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
                    console.log("Index in Region ", groupDataModel.data(indexPath).name);
                    regionspage.region = groupDataModel.data(indexPath).name;
                    city_model.populate(sourcespage.source, groupDataModel.data(indexPath).key);
                    var newPage = nextpage3.createObject();
                    rootWindow.push(newPage);
                }
            }
        }       
    }
}
