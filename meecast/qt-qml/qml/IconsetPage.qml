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
       		   id: nextpage
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
					for (var a in  Config.icon_list() ){
						groupDataModel.insert( {"name" : Config.icon_list()[a], "number" : a});
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
				onTriggered: {             
					console.log("Index ", groupDataModel.data(indexPath).qml);
//					nextpage.source = groupDataModel.data(indexPath).qml;
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

