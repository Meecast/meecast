import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: countries 
    objectName: "countriespage"
    property int margin: 16
    property string country: ""
    property string first_string: ""
    property string second_string: ""
    property string key: ""

    content:  Container{
        id: absoluteLayoutContainer
        background: Color.White
        layout: AbsoluteLayout {}
        attachedObjects: [
            ComponentDefinition {
                id: nextpage2
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
        TextField {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 90
            }
            hintText: Config.tr ("Filter by...")
            onTextChanging: { 
                groupDataModel.clear();
                if (text != ""){
                    for (var a = 0; a < country_model.rowCount(); a++) {
                        first_string = text.toLocaleUpperCase()
                        second_string =  country_model.get(a).name.toLocaleUpperCase();
                        if (second_string.indexOf(first_string) >= 0){
                            groupDataModel.insert( {"name" : country_model.get(a).name, "number" : a, "key" : country_model.get(a).key });
                        }
                    }
                }else{
                    for (var a = 0; a < country_model.rowCount(); a++) {
                        groupDataModel.insert( {"name" : country_model.get(a).name, "number" : a, "key" : country_model.get(a).key });
                    }
                }
            }
        }
        Container{
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 160
            }
            preferredHeight: 970 
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
                    console.log ("Country Size ", country_model.rowCount());
                    for (var a = 0; a < country_model.rowCount(); a++) {
                        groupDataModel.insert( {"name" : country_model.get(a).name, "number" : a, "key" : country_model.get(a).key });
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
                    console.log("Index in Country ", groupDataModel.data(indexPath).name);
                    console.log("Source ", source);
                    nextpage2.source = "RegionPage.qml" 
                    countries.country =  groupDataModel.data(indexPath).name;
                    countries.key =  groupDataModel.data(indexPath).key;
                    region_model.populate(sourcespage.source, groupDataModel.data(indexPath).key);
                    var newPage = nextpage2.createObject();
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
            text:  Config.tr("Select country") 
            textStyle {
                base: SystemDefaults.TextStyles.TitleText
                color: Color.White
            }
        }


    }
}
