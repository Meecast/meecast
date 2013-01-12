import QtQuick 1.0
import bb.cascades 1.0
import bb.system 1.0

Page {
    id: stations
    objectName: "stationspage"
    property int margin: 16
    property int removedStation: -1
    //Config {id: config1}
    
    actions: [
        ActionItem {
            title:  Config.tr("Add")
            imageSource: "asset:///button_icons/add_def.png"
            onTriggered: {  
                var newPage = nextpage.createObject();
                rootWindow.push(newPage);
            }
            ActionBar.placement: ActionBarPlacement.OnBar
        }   
    ]


    content:  Container{
        id: absoluteLayoutContainer
        background: Color.White
        layout: AbsoluteLayout {}
        attachedObjects: [
            ComponentDefinition {
                id: nextpage
                source: "SourcePage.qml" 
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
                    for (var a in  Config.stations() ){
                        groupDataModel.insert( {"name" : Config.stations()[a], "number" : a});
                    }
                    Qt.removeStation = removeStation; 
                    Qt.removedStation = removedStation;
                    Qt.dialog = dialog;
                }     
                attachedObjects: [
                    SystemDialog {
                        id: dialog
                        body: Config.tr("Delete location?") 
                        confirmButton.enabled: true
                        confirmButton.label: Config.tr("Yes")
                        cancelButton.enabled: true
                        cancelButton.label: Config.tr("No")
                        onFinished: {
                            if (dialog.result == SystemUiResult.ConfirmButtonSelection ){
                                Qt.removeStation(Qt.removedstation);
                                groupDataModel.clear();
                                for (var a in  Config.stations() ){
                                    groupDataModel.insert( {"name" : Config.stations()[a], "number" : a});
                                }

                            }
                        }
                    }
                ]
      
                listItemComponents: [
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
                                      base: SystemDefaults.TextStyles.TitleText
                                      color: Color.White
                                  }
                            }
                            Button {
                                text: "X"
                                preferredWidth: 60 
                                preferredHeight: 60 
                                horizontalAlignment: HorizontalAlignment.Right
                                verticalAlignment: VerticalAlignment.Center
                                onClicked: {
                                    Qt.dialog.title = ListItemData.name 
                                    Qt.removedstation = ListItemData.number
                                    Qt.dialog.show();
                                }
                            }
                         }
                    }
                ]
                function removeStation(index) {
                    Config.removeStation(index);
                }
                onTriggered: {             
//                  console.log("Index ", groupDataModel.data(indexPath).qml);
//                  nextpage.source = groupDataModel.data(indexPath).qml;
                }
            }
        }       
        Label {
            layoutProperties: AbsoluteLayoutProperties {
                positionX: 0
                positionY: 0
            }
            preferredWidth: 768
            text:  Config.tr("Stations") 
            textStyle {
                base: SystemDefaults.TextStyles.BigText
                color: Color.White
            }
        }


    }
}
