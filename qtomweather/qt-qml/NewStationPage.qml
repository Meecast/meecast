import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: newstation
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }
        ToolIcon {
            iconId: "toolbar-done"
            onClicked: {
                //save station
                if (city_dlg.selectedIndex >= 0){
                    Config.saveStation(city_dlg.selectedIndex, city_dlg.selectedText,
                                   region_dlg.selectedIndex, region_dlg.selectedText,
                                   country_dlg.selectedIndex, country_dlg.selectedText,
                                   source_dlg.selectedIndex, source_dlg.selectedText);
                }
                pageStack.pop();
            }
        }
    }
    orientationLock: PageOrientation.LockPortrait
    Item {
        id: mainitem

        MySelectionDialog {
            id: source_dlg
            titleText: Config.tr("Select source")
            selectedIndex: -1
            model: Config.Sources()
            onAccepted: {
                country_dlg.model = Config.Countries(selectedText);
            }
        }
        MySelectionDialog {
            id: country_dlg
            titleText: Config.tr("Select country")
            selectedIndex: -1
            model: []
            onAccepted: {
                region_dlg.model = Config.Regions(selectedIndex);
            }
        }
        MySelectionDialog {
            id: region_dlg
            titleText: Config.tr("Select region")
            selectedIndex: -1
            model: []
            onAccepted: {
                city_dlg.model = Config.Cities(country_dlg.selectedIndex, selectedIndex);
            }
        }
        MySelectionDialog {
            id: city_dlg
            titleText: Config.tr("Select city")
            selectedIndex: -1
            model: []
            onAccepted: {

            }
        }

        //parent: appPage.content
        anchors.fill: parent
        Column {
            width: parent.width
            spacing: 20
            Button {
                text: source_dlg.selectedIndex >= 0 ?
                        source_dlg.selectedText :
                        Config.tr("Select source")
                onClicked: {
                    source_dlg.open();
                }
            }
            Button {
                text: country_dlg.selectedIndex >= 0 ?
                        country_dlg.selectedText :
                        Config.tr("Select country")
                onClicked: {
                    country_dlg.open();
                }
            }
            Button {
                text: region_dlg.selectedIndex >= 0 ?
                        region_dlg.selectedText :
                        Config.tr("Select region")
                onClicked: {
                    region_dlg.open();
                }
            }
            Button {
                text: city_dlg.selectedIndex >= 0 ?
                        city_dlg.selectedText :
                        Config.tr("Select city")
                onClicked: {
                    city_dlg.open();
                }
            }
        }
    }
}
