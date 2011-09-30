import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: units
    property int margin: 16
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                //menu.close();
                pageStack.pop();
            }
        }
    }
    orientationLock: PageOrientation.LockPortrait

    function getIndex(model, value)
    {
        var i=0;
        while (i<model.length && model[i]!=value) i++;
        if (i == model.length) return -1;
        else return i;
    }

    MySelectionDialog {
        id: temperature_dlg
        model: Config.temperature_list()
        titleText: Config.tr("Temperature units")
        selectedIndex: -1
        onAccepted: {
            Config.temperature_unit(selectedText);
        }

    }
    MySelectionDialog {
        id: wind_dlg
        model: Config.windspeed_list()
        titleText: Config.tr("Wind speed units")
        selectedIndex: -1
        onAccepted: {
            Config.windspeed_unit(selectedIndex);
        }

    }
    Rectangle{
        anchors.fill: parent
        anchors.top: title_rect.bottom
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            color: "#999999"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 274
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 274
            color: "black"
        }
    Column {
        anchors.fill: parent
        //spacing: 20

        Label {
            text: Config.tr("Temperature units")
            height: 80
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        ButtonColumn {
            width: parent.width
            platformStyle: ButtonStyle {
                horizontalAlignment: Text.AlignLeft
            }
            Button {
                text: Config.tr("Celcius")
                checked: (Config.temperatureunit == 'C')

                onClicked: {
                    Config.temperature_unit('C');
                }
            }
            Button {
                text: Config.tr("Farenheit")
                checked: (Config.temperatureunit == 'F')
                onClicked: {
                    Config.temperature_unit('F');
                }
            }
        }
        /*
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: Config.tr("Temperature")+": "+Config.temperatureunit
            onClicked: {
                temperature_dlg.open();
            }
            Component.onCompleted: {
                temperature_dlg.selectedIndex = units.getIndex(temperature_dlg.model, Config.temperatureunit)
            }
        }
        */
        Label {
            text: Config.tr("Wind speed units")
            height: 80
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
        ButtonColumn {
            width: parent.width
            platformStyle: ButtonStyle {
                horizontalAlignment: Text.AlignLeft
            }
            Button {
                text: Config.tr("m/s")
                checked: (Config.windspeedunit == 'm/s')
                onClicked: {
                    Config.windspeed_unit('m/s');
                }
            }
            Button {
                text: Config.tr("km/h")
                checked: (Config.windspeedunit == 'km/h')
                onClicked: {
                    Config.windspeed_unit('km/h');
                }
            }
            Button {
                text: Config.tr("mi/h")
                checked: (Config.windspeedunit == 'mi/h')
                onClicked: {
                    Config.windspeed_unit('mi/h');
                }
            }

        }/*
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: Config.tr("Wind speed")+": "+Config.windspeedunit
            onClicked: {
                wind_dlg.open();
            }
            Component.onCompleted: {
                wind_dlg.selectedIndex = units.getIndex(wind_dlg.model, Config.windspeedunit)
            }
        }*/
    }
}
    Rectangle {
        id: title_rect
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        width: parent.width - 2*margin
        height: 80
        color: "black"
        Label {
            id: title
            anchors.fill: parent
            color: "white"
            text: Config.tr("Measurement units")
            font.family: "Nokia Pure Light"
            font.pixelSize: 30
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }
    }

}
