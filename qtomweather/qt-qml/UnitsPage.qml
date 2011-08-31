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
        console.log(model.length+" "+model[0]+" "+value);
        while (i<model.length && model[i]!=value) i++;
        console.log(i);
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

    Label {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        text: Config.tr("Units")
        font.pixelSize: 28
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    Column {
        anchors.top: title.bottom
        anchors.topMargin: 30
        width: parent.width
        spacing: 20

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
    }

}
