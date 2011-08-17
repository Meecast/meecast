//import Qt 4.7
import QtQuick 1.1
//import "." 1.0
import com.nokia.meego 1.0

PageStackWindow {
    id: rootWindow
    platformStyle: PageStackWindowStyle { id: defaultStyle }

    initialPage: WeatherPage {}

    /*
    ToolBarLayout {
        id: toolbar
        visible: false

        ToolIcon {
            iconId: "toolbar-back"
            onClicked: {
                menu.close();
                pageStack.pop();
            }
        }
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: {
                menu.status == (DialogStatus.Closed) ? menu.open() : menu.close()
            }
        }
    }*/

}
