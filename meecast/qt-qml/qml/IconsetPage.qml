import QtQuick 1.0
import bb.cascades 1.0


Page {
    
    id: iconset 
    property int screen_width  :  768
    property int screen_height : 1280    

   

    content: 
    Container{
        id: absoluteLayoutContainer
        background: Color.White

        layout: AbsoluteLayout {}
        attachedObjects: [
            ComponentDefinition {
               id: nextpage5
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
              
        Container {
            layoutProperties: 
            AbsoluteLayoutProperties {
                positionY: 90
            }
            preferredHeight: 1050
            attachedObjects: [
                GroupDataModel {
                    id: groupDataModel
                    sortingKeys: ["number"]
                    grouping: ItemGrouping.None
                }
            ]
            ListView {
                id: listview
                property string first_selection: "-1"
                dataModel: groupDataModel 
                onCreationCompleted: {
                    for (var a in  Config.icon_list() ){
                        groupDataModel.insert( {"name" : Config.icon_list()[a], "number" : a,
                        "pict" : Config.iconspath + "/" + Config.icon_list()[a] + "/" + "28.png",
                        "active": Config.icon_list()[a] == Config.iconset  ? true : false });
                        if (Config.icon_list()[a] == Config.iconset ){
                            select(a);
                            first_selection = a
                        }
                    }
                }     
         
                listItemComponents: [
                     ListItemComponent {
                         type: "item"
                         id: listitemcomp
                         Container {
                            layout: DockLayout {}
                            Container {
                                id: item_container
                                background: Color.White
                                opacity: ListItemData.active ? 0.2 : 0.0
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
                             }
                             Label {                 
                                  text: ListItemData.name
                                  preferredWidth: 768
                                  horizontalAlignment:  HorizontalAlignment.Fill
                                  verticalAlignment: VerticalAlignment.Center
                                  textStyle {
                                      base: SystemDefaults.TextStyles.TitleText
                                      color: Color.White
                                  }
                             }
                             ImageView {
                                    imageSource: ListItemData.pict
                                    preferredWidth: 128
                                    preferredHeight: 128
                                    horizontalAlignment: HorizontalAlignment.Right                
                             }
                            function setHighlight (highlighted) {
                                if (highlighted) {
                                    item_container.opacity = 0.2;
                                } else {
                                     item_container.opacity = 0.0;
                                }
                            }      
                            ListItem.onActivationChanged: {
                                setHighlight (ListItem.active);
                            }
                             ListItem.onSelectionChanged: {
                                setHighlight (ListItem.selected);
                             }
                         }
                    }
                ]

                onTriggered: {             
                    if (first_selection != "-1"){
                        select([first_selection]);
                        first_selection = "-1";
                    }
                    clearSelection();
                    select(indexPath);
                    Config.set_iconset(groupDataModel.data(indexPath).name)
                    settings2_absoluteLayoutContainer.update_list()
                    rootWindow.navigateTo(settings2);
                }

            }
        }
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredWidth: 768
            text: Config.tr("Select the iconset")
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
            }
        }
    }
}

