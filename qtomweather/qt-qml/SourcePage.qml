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

    
    Rectangle {
      id: rect
      anchors.top: labelrect.bottom 
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      width: parent.width 
      color: Qt.rgba(0, 0, 0, 0.1)


      ListView {
          id: sourcelist
          anchors.fill: parent
          model: source_model
          anchors.top: rect.top 
          anchors.bottom: rect.bottom 
   
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
    Rectangle {
      id: labelrect
      anchors.top: parent.top
      anchors.left: parent.left
      width: parent.width
      color: "black" 
      border.color: "black"
      border.width: 3 
      height: 32
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
    }


}

