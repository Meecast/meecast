pragma Singleton
import QtQml 2.0
import QtQuick.Window 2.2
  QtObject {
      property variant primaryColor: "ghostwhite"
      property variant secondaryColor: "darkgrey"
      property int paddingLarge: 50
      property int paddingSmall: 20
      property real ratio: Screen.devicePixelRatio
      property int large_FontPointSize: 36*ratio
      property int huge_PixelSize: 54*ratio
      property int itemSizeSmall: 60*ratio
  }


