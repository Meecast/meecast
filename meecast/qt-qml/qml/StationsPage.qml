import QtQuick 1.0
import bb.cascades 1.0

Page {
    id: stations
    objectName: "stationspage"
    property int margin: 16
    property int removedStation: -1
    property string removedStationName: ""
    //Config {id: config1}

	actions: [
		ActionItem {
			title:  Config.tr("Add")
			onTriggered: { 	
				var newPage = nextpage.createObject();
				rootWindow.push(newPage);
			}
			ActionBar.placement: ActionBarPlacement.OnBar
		}	
	]

	attachedObjects: [
		Dialog {
			id: yesnoDialog
			Container {
				horizontalAlignment: HorizontalAlignment.Fill
				verticalAlignment: VerticalAlignment.Fill
				Button {
					horizontalAlignment: HorizontalAlignment.Center
					text: Config.tr("No")
					onClicked: myDialog.close()
				}
		   }
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
						console.log("Stationq11111 ", Config.stations()[a]);
						groupDataModel.insert( {"name" : Config.stations()[a], "number" : a});
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
										yesnoDialog.open();
									}
							}

						 }
					}
				]
				onTriggered: {             
					console.log("Index ", groupDataModel.data(indexPath).qml);
					nextpage.source = groupDataModel.data(indexPath).qml;
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
				base: SystemDefaults.TextStyles.TitleText
				color: Color.White
			}
		}


	}
}
