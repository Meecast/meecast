import Qt 4.7
//import QtQuick 1.1
//import Qt.labs.components 1.0
import com.nokia.meego 1.0

Page {
    id: sourcepage
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                pageStack.pop();
            }
        }

    }
    orientationLock: PageOrientation.LockPortrait

    
    Label {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        text: Config.tr("Sources")
        font.pixelSize: 28
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    ListView {
        id: sourcelist
        anchors.fill: parent
        model: source_model

        delegate: Component {
            id: listdelegate
            Item {
                width: parent.width
                height: 50
                Text {
                    text: model.name
                    color: "white"
                    font.pointSize: 30
                    height: 50
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        country_model.populate(model.name);
                        pageStack.push(Qt.resolvedUrl("CountryPage.qml"),
                                       {source: model.name, source_id: index}
                                       );
                    }
                }
            }
        }
    }

}

