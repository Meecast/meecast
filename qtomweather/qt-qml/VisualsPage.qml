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
        id: icon_dlg
        model: Config.icon_list()
        titleText: Config.tr("Icons")
        selectedIndex: -1
        onAccepted: {
            Config.set_iconset(selectedText);
        }

    }

    Label {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        text: Config.tr("Visuals")
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
            text: Config.tr("Iconset")+": "+Config.iconset
            onClicked: {
                icon_dlg.open();
            }
            Component.onCompleted: {
                icon_dlg.selectedIndex = units.getIndex(icon_dlg.model, Config.iconset);
            }
        }
        Button {
            anchors.horizontalCenter: parent.horizontalCenter
            text: Config.tr("FullScreen")
            checkable: true
            checked: rootWindow.showStatusBar
            onClicked: {
                rootWindow.showStatusBar = !rootWindow.showStatusBar;
            }
        }

    }

}

