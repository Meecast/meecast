import Qt 4.7
import "search.js" as Search

Item {
    id: adaptivepanel

    height: buttongrid.height > controlbuttons.height ? buttongrid.height : controlbuttons.height

    signal filterUpdated()

    property int buttonWidth: width / 5 - buttongrid.spacing
    property int buttonHeight: buttonWidth
    property int buttonMargin: 1

    Column {
        id: controlbuttons
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: buttonMargin

        PanelButton {
            id: backButton
            width: buttonWidth
            height: buttonHeight

            // Show back button only when going back is possible (i.e filter length > 0)
            opacity: filter.length > 0 ? 1 : 0

            onClicked: {
                filter = filter.substring(0, filter.length-1)
                Search.fillFilterModel()
                Search.updateFilterModel()
                filterUpdated()
            }

            Image {
                source: "gfx/left.png"
                anchors.centerIn: parent
            }

            Behavior on opacity { NumberAnimation { duration: 500 }}
        }

        PanelButton {
            id: closeButton
            width: buttonWidth
            height: buttonHeight

            onClicked: adaptiveroot.state = "minimized"

            Image {
                source: "gfx/down.png"
                anchors.centerIn: parent
            }
        }
    }

    Grid {
        id: buttongrid
        columns: Math.floor(parent.width/buttonWidth) - 1
        spacing: buttonMargin
        x: 0
        y: 0
        anchors.bottom: parent.bottom

        Repeater {
            model: Search.filterButtonCount()

            BorderImage {
                width: buttonWidth
                height: buttonHeight
                border {
                    top: 10
                    bottom: 10
                    left: 10
                    right: 10
                }
                source: filterbuttonarea.pressed ? "gfx/button_pressed.png" : "gfx/button.png"

                smooth: true
                Text {
                    id: filterbuttontext
                    text: Search.filterButtonText(index)
                    font.pointSize: 20
                    anchors.centerIn: parent
                    smooth: true
                }
                MouseArea {
                    id: filterbuttonarea
                    anchors.fill: parent
                    onClicked: {
                        filter = filter + filterbuttontext.text

                        Search.updateFilterModel()
                        filterUpdated()
                    }
                }
            }
        }

        move: Transition {
                 NumberAnimation {
                     properties: "x,y"
                     duration: 500
                     easing.type: Easing.InOutCubic
                 }
             }
    }
}
