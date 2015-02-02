/****************************************************************************
**
** Copyright (C) 2013 Jolla Ltd.
** Contact: Vesa Halttunen <vesa.halttunen@jollamobile.com>
**
****************************************************************************/

import QtQuick 2.0
import org.nemomobile.lipstick 0.1
import Sailfish.Silica 1.0
import com.jolla.lipstick 0.1
import org.nemomobile.time 1.0
import "../notifications"
import "../scripts/desktop.js" as Desktop

MouseArea {
    id: lockScreen
    property alias clockHeight: clock.height
    property bool fullyOnDisplay: dashboard.contentY == 0 && Lipstick.compositor.homeVisible
    property bool onDisplay: dashboard.contentY < dashboard.lockscreenAndStatusAreaHeight && Lipstick.compositor.homeVisible
    property bool recentlyOnDisplay

    // Allows start-up wizard to not become backgrounded when the display turns off/on
    property int forceTopWindowProcessId: -1

    // Use a binding rather than a Connections element to shortcut spurious change signals.
    property bool lockscreenVisible: lipstickSettings.lockscreenVisible

    // Suffix that should be added to all theme icons that are shown in low power mode
    property string iconSuffix: lipstickSettings.lowPowerMode ? ('?' + Theme.highlightColor) : ''

    // Text color of items that are shown in low power mode
    property color textColor: lipstickSettings.lowPowerMode ? Theme.highlightColor : Theme.primaryColor

    property Item accessNotificationsHint

    onOnDisplayChanged: {
        if (onDisplay) {
            if (desktop.animating) {
                onTimer.start()
            } else {
                recentlyOnDisplay = onDisplay
            }

            lockScreen.state = "showDateImmediatelyWithoutTimer"
        }
    }

    Timer {
        id: onTimer
        interval: 200
        onTriggered: lockScreen.recentlyOnDisplay = lockScreen.onDisplay
    }
    Timer {
        interval: 3000
        running: !lockScreen.onDisplay
        onTriggered: lockScreen.recentlyOnDisplay = lockScreen.onDisplay
    }
    Timer {
        id: offTimer
        interval: 50
        running: false
        onTriggered: {
            if (forceTopWindowProcessId < 0
                    || Lipstick.compositor.topmostWindow == null
                    || Lipstick.compositor.topmostWindow == forceTopWindowProcessId) {
                dashboard.snapToPage(0, true);
                Lipstick.compositor.setCurrentWindow(null);
                desktop.closeApplicationEnabled = false
                desktop.removeApplicationEnabled = false
            }
        }
    }

    onLockscreenVisibleChanged: {
        if (!lockscreenVisible) {
            if (dashboard.currentIndex === 0) {
                if (Desktop.instance.ambienceOnLockscreen) {
                    Desktop.instance.ambienceOnLockscreen = false
                } else {
                    // lockscreen visible, but we don't want to see it.
                    // reset view to switcher/launcher
                    dashboard.snapToPage(1);
                }
            }
        } else {
            // lockscreen enabled. make sure we're already there.
            offTimer.restart()
        }
    }

    Clock {
        id: clock
        anchors {
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }

    ConnectionStatusIndicator {
        anchors {
            bottom: clock.verticalCenter
            bottomMargin: Math.round(Theme.paddingSmall/2)
            left: parent.left
            leftMargin: Theme.smallIcons ? Theme.paddingSmall : Theme.paddingLarge
        }
    }

    Image {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        source: "image://theme/graphic-wallpaper-dimmer"
    }

    Item {
        id: networkNameAndDateLabels
        anchors {
            top: parent.top
            topMargin: Theme.paddingLarge
        }
        width: parent.width
        height: Math.max(cellularNetworkNameStatusIndicator.height, dateLabel.height)

        CellularNetworkNameStatusIndicator {
            id: cellularNetworkNameStatusIndicator
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 1 - dateLabel.opacity
        }

        Label {
            id: dateLabel
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width - (Screen.width / 5 * 2)
            text: {
                var dateString = Format.formatDate(wallClock.time, Format.DateFull)
                return dateString.charAt(0).toUpperCase() + dateString.substr(1)
            }
            color: Theme.highlightColor
            horizontalAlignment: Text.AlignHCenter
            maximumLineCount: 2
            wrapMode: Text.WordWrap
            WallClock {
                id: wallClock
                enabled: lockScreen.onDisplay
                updateFrequency: WallClock.Day
            }
            Behavior on opacity {
                id: dateOpacityBehavior
                FadeAnimation { }
            }
        }
    }
    Item{
        id: weatherData
        property QtObject meecastData 
        property bool active: true
        visible: true
        Component.onCompleted: {
            meecastData = Qt.createQmlObject('import QtQuick 2.0; import org.meecast.data 1.0 as Meecast; Meecast.Data {}', weatherData)
        }
        anchors {
            top: networkNameAndDateLabels.bottom
            topMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.paddingMedium
        }
        width: 415
        height: 140  

        Rectangle {
            id: background
            anchors.fill: parent
            color: "grey"
            opacity: lipstickSettings.lowPowerMode ? 0.0 : 0.3
            radius: 1
        }

        Rectangle {
            id: background_border
            anchors.left: parent.left
            anchors.fill: parent
            border.color: lipstickSettings.lowPowerMode ? Theme.highlightColor : Theme.secondaryColor
            border.width: 1
            color: "transparent"
            radius: 1
        }

        Rectangle {
            id: first_item 
            anchors.left: parent.left
            color: "transparent"
            width: parent.height 
            height: parent.height 

            Image {
                id: icon_1
                source: weatherData.meecastData.forecastdata["item1_icon"]
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                opacity: 0.4
                height: parent.height - Theme.paddingSmall
                width: parent.height - Theme.paddingSmall
            }

            Label {
                id: temperature_1 
                anchors.top: parent.top
                anchors.topMargin: -Theme.paddingSmall
                color: lockScreen.textColor
                width: parent.width - Theme.paddingSmall
                font.pixelSize: Theme.fontSizeExtraLarge + 5
                horizontalAlignment: Text.AlignHCenter
                text: {
                    if (weatherData.meecastData.forecastdata["item1_temperature"] && weatherData.meecastData.forecastdata["item1_temperature"] != "N/A"){
                        return weatherData.meecastData.forecastdata["item1_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                    }else{
                        return temperature_1.text = weatherData.meecastData.forecastdata["item1_temperature_low"] +  '°' + weatherData.meecastData.forecastdata["temperatureunit"] + "\n"+  weatherData.meecastData.forecastdata["item1_temperature_high"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                    }
                }
            }
        }
        Rectangle {
            id: second_item 
            anchors.left: first_item.right
            anchors.bottom: background.bottom
            color: "transparent"
            width: parent.height*2/3
            height: parent.height*2/3

            Image {
                id: icon_2
                source: weatherData.meecastData.forecastdata["item2_icon"]
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                opacity: 1.0 
                height: weatherData.height/2 
                width: weatherData.height/2 

            }
            Label {
                id: day_name_2 
                anchors.bottom: icon_2.top 
                anchors.bottomMargin: -Theme.paddingSmall
                color: lockScreen.textColor
                font.pixelSize: Theme.fontSizeMedium/2 
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                text: weatherData.meecastData.forecastdata["item2_short_day_name"]
            }
            Label {
                id: temperature_2 
                anchors.bottom: parent.bottom
                color: lockScreen.textColor
                font.pixelSize: Theme.fontSizeExtraSmall - 3
                horizontalAlignment: Text.AlignHCenter
                width: parent.width
                text: {
                    if ( weatherData.meecastData.forecastdata["item2_temperature_high"] == "N/A" || weatherData.meecastData.forecastdata["item2_temperature_low"] == "N/A" ){   
                        return temperature_2.text = weatherData.meecastData.forecastdata["item2_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                    }else{
                        return weatherData.meecastData.forecastdata["item2_temperature_low"] + "/"+  weatherData.meecastData.forecastdata["item2_temperature_high"]  + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                    }
                }
            }

        }
        Rectangle {
            id: thrid_item 
            anchors.left: second_item.right
            anchors.bottom: background.bottom
            color: "transparent"
            width: parent.height*2/3
            height: parent.height*2/3

            Image {
                id: icon_3
                source: weatherData.meecastData.forecastdata["item3_icon"]
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                opacity: 1.0 
                height: weatherData.height/2 
                width: weatherData.height/2 
            }
            Label {
                id: day_name_3 
                anchors.bottom: icon_3.top 
                anchors.bottomMargin: -Theme.paddingSmall
                color: lockScreen.textColor
                font.pixelSize: Theme.fontSizeMedium/2 
                horizontalAlignment: Text.AlignHCenter
                text: weatherData.meecastData.forecastdata["item3_short_day_name"]
                width: parent.width
            }
            Label {
                id: temperature_3
                anchors.bottom: parent.bottom
                color: lockScreen.textColor
                font.pixelSize: Theme.fontSizeExtraSmall - 3 
                horizontalAlignment: Text.AlignHCenter
                text: weatherData.meecastData.forecastdata["item3_temperature"] + '°'+ weatherData.meecastData.forecastdata["temperatureunit"]
                width: parent.width
            }
        }
        Rectangle {
            id: forth_item 
            anchors.left: thrid_item.right
            anchors.bottom: background.bottom
            color: "transparent"
            width: parent.height*2/3
            height: parent.height*2/3

            Image {
                id: icon_4
                source: weatherData.meecastData.forecastdata["item4_icon"]
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                opacity: 1.0 
                height: weatherData.height/2 
                width: weatherData.height/2 
            }
            Label {
                id: day_name_4
                anchors.bottom: icon_4.top
                anchors.bottomMargin: -Theme.paddingSmall
                color: lockScreen.textColor
                font.pixelSize: Theme.fontSizeMedium/2 
                horizontalAlignment: Text.AlignHCenter
                text: weatherData.meecastData.forecastdata["item4_short_day_name"]
                width: parent.width
            }
            Label {
                id: temperature_4
                anchors.bottom: parent.bottom
                color: lockScreen.textColor
                font.pixelSize: Theme.fontSizeExtraSmall - 3
                horizontalAlignment: Text.AlignHCenter
                text: weatherData.meecastData.forecastdata["item4_temperature"] + '°'+ weatherData.meecastData.forecastdata["temperatureunit"]
                width: parent.width
            }
        }

        Rectangle{
            id: header_rect 
            anchors.right: parent.right
            anchors.top: parent.top
            height: parent.height*1/4
            color: "transparent"
            width: parent.width - first_item.width 
            Label {
                id: station_name 
                height: parent.height
                anchors.right: last_update.left
                anchors.rightMargin: Theme.paddingSmall
                anchors.left: parent.left
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: weatherData.meecastData.nameString 
                font.pixelSize: Theme.fontSizeExtraSmall
                color: lipstickSettings.lowPowerMode ? Theme.highlightColor : Theme.secondaryColor
            }
            Label {
                id: last_update 
                height: parent.height
                anchors.right: parent.right
                verticalAlignment: Text.AlignVCenter
                anchors.rightMargin: Theme.paddingSmall
                text: weatherData.meecastData.forecastdata["last_update"]
                font.pixelSize: Theme.fontSizeExtraSmall - 5
                color: lipstickSettings.lowPowerMode ? Theme.highlightColor : Theme.secondaryColor
            }
        }
        Connections {
            target: weatherData.meecastData 
            onRefreshWidget: {            
                console.log("Refresh MeeCast widget !!!!!!!!!!!!!!")
                station_name.text = weatherData.meecastData.nameString 
                last_update.text = weatherData.meecastData.forecastdata["last_update"]
                if (weatherData.meecastData.forecastdata["item1_temperature"] && weatherData.meecastData.forecastdata["item1_temperature"] != "N/A"){
                    temperature_1.text = weatherData.meecastData.forecastdata["item1_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }else{
                     temperature_1.text = weatherData.meecastData.forecastdata["item1_temperature_low"] +  '°' + weatherData.meecastData.forecastdata["temperatureunit"] + "\n"+  weatherData.meecastData.forecastdata["item1_temperature_high"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }

                if ( weatherData.meecastData.forecastdata["item2_temperature_high"] == "N/A" || weatherData.meecastData.forecastdata["item2_temperature_low"] == "N/A" ){   
                    temperature_2.text = weatherData.meecastData.forecastdata["item2_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }else{
                    temperature_2.text = weatherData.meecastData.forecastdata["item2_temperature_low"] + "/"+  weatherData.meecastData.forecastdata["item2_temperature_high"]  + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }

                if ( weatherData.meecastData.forecastdata["item3_temperature_high"] == "N/A" || weatherData.meecastData.forecastdata["item3_temperature_low"] == "N/A" ){   
                    temperature_3.text = weatherData.meecastData.forecastdata["item3_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }else{
                    temperature_3.text = weatherData.meecastData.forecastdata["item3_temperature_low"] + "/"+  weatherData.meecastData.forecastdata["item3_temperature_high"]  + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }
 
                if ( weatherData.meecastData.forecastdata["item4_temperature_high"] == "N/A" || weatherData.meecastData.forecastdata["item4_temperature_low"] == "N/A" ){   
                    temperature_4.text = weatherData.meecastData.forecastdata["item4_temperature"] + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }else{
                    temperature_4.text = weatherData.meecastData.forecastdata["item4_temperature_low"] + "/"+  weatherData.meecastData.forecastdata["item4_temperature_high"]  + '°' + weatherData.meecastData.forecastdata["temperatureunit"]
                }
                
                icon_1.source = weatherData.meecastData.forecastdata["item1_icon"]
                icon_2.source = weatherData.meecastData.forecastdata["item2_icon"]
                icon_3.source = weatherData.meecastData.forecastdata["item3_icon"]
                icon_4.source = weatherData.meecastData.forecastdata["item4_icon"]
                day_name_2.text = weatherData.meecastData.forecastdata["item2_short_day_name"]
                day_name_3.text = weatherData.meecastData.forecastdata["item3_short_day_name"]
                day_name_4.text = weatherData.meecastData.forecastdata["item4_short_day_name"]
            }
        }
    }
    SimToolkitIdleModeIndicator {
        anchors {
            top: networkNameAndDateLabels.bottom
            topMargin: Theme.paddingMedium
            horizontalCenter: parent.horizontalCenter
        }
        width: parent.width - 2*(Theme.iconSizeLarge + Theme.paddingLarge)
    }

    ProfileStatusIndicator {
        id: profileStatusIndicator
        anchors {
            bottom: clock.verticalCenter
            bottomMargin: Math.round(Theme.paddingSmall/2)
            right: parent.right
            rightMargin: Theme.paddingLarge
        }
    }

    AlarmStatusIndicator {
        anchors {
            top: clock.verticalCenter
            topMargin: Math.round(Theme.paddingSmall/2)
            right: parent.right
            rightMargin: Theme.paddingLarge
        }
    }

    BluetoothStatusIndicator {
        anchors {
            top: clock.verticalCenter
            topMargin: Math.round(Theme.paddingSmall/2)
            left: parent.left
            leftMargin: Theme.paddingLarge
        }
    }

    JollaNotificationListModel {
        id: horizontalNotificationListModel
        categoryFilter: "(x-nemo.system-update)"
    }

    JollaNotificationListModel {
        id: verticalNotificationListModel
        categoryFilter: "^((?!(x-nemo.system-update)).*)$"
        minimumPriority: 100
    }

    Column {
        width: parent.width
        y: Theme.paddingLarge

        HorizontalNotificationList {
            id: horizontalNotificationList
            model: horizontalNotificationListModel
            width: parent.width
            iconOnly: true
            onClicked: showAccessNotificationsHint()
        }
        NotificationLockScreenView {
            id: verticalNotificationList
            width: parent.width
            model: verticalNotificationListModel
            notificationLimit: 4
            height: Math.min(verticalNotificationListModel.itemCount * notificationHeight,
                             notificationLimit * notificationHeight)
            iconSuffix: lockScreen.iconSuffix
            textColor: lockScreen.textColor
            onClicked: showAccessNotificationsHint()
        }
        Image {
            x: Theme.paddingLarge
            source: "image://theme/icon-lock-more" + lockScreen.iconSuffix
            /* Hide the more notifications icon when there aren't too many notifications */
            opacity: verticalNotificationListModel.itemCount <= verticalNotificationList.notificationLimit ? 0 : 1
            Behavior on opacity { FadeAnimation {} }
            visible: opacity > 0.0
        }
    }

    function showAccessNotificationsHint() {
        if (!accessNotificationsHint) {
            var component = Qt.createComponent(Qt.resolvedUrl("AccessNotificationsHint.qml"))
            if (component.status == Component.Ready) {
                accessNotificationsHint = component.createObject(desktop)
                accessNotificationsHint.finished.connect(function() {
                    lockScreen.accessNotificationsHint = null
                })
            } else {
                console.warn("AccessNotificationsHint.qml instantiation failed " + component.errorString())
            }
        }
    }

    OngoingCall {
        anchors.bottom: clock.top
    }

    SneakPeekHint {
        id: sneakPeekHint
    }

    Connections {
        target: Lipstick.compositor
        onDisplayOff: {
            if (lockscreenVisible && onDisplay && dashboard.contentY > 0) {
                dashboard.cancelFlick()
                dashboard.snapToPage(0, true)
            }
        }

        onDisplayAboutToBeOn: {
            lockScreen.state = "showDateImmediately"
            sneakPeekHint.sneakPeekActive = lipstickSettings.lowPowerMode
        }
        onDisplayAboutToBeOff: sneakPeekHint.sneakPeekActive = false
    }

    Connections {
        target: lipstickSettings
        onLowPowerModeChanged: if (!lipstickSettings.lowPowerMode) sneakPeekHint.sneakPeekActive = false
    }

    onFullyOnDisplayChanged: {
        if (fullyOnDisplay && !hideDateTimer.running) {
            lockScreen.state = "showDate"
        }
    }

    Timer {
        id: hideDateTimer
        interval: 3000
        onTriggered: {
            var indicator = cellularNetworkNameStatusIndicator
            if (!lipstickSettings.lowPowerMode && (indicator.homeNetwork || indicator.visitorNetwork)) {
                lockScreen.state = "showCellularNetworkName"
            }
        }
    }

    states: [
        State {
            name: "showDateImmediatelyWithoutTimer"
            StateChangeScript {
                script: {
                    dateOpacityBehavior.enabled = false
                    dateLabel.opacity = 1
                    dateOpacityBehavior.enabled = true
                }
            }
        },
        State {
            name: "showDateImmediately"
            StateChangeScript {
                script: {
                    dateOpacityBehavior.enabled = false
                    dateLabel.opacity = 1
                    dateOpacityBehavior.enabled = true
                    hideDateTimer.start()
                }
            }
        },
        State {
            name: "showDate"
            StateChangeScript {
                script: {
                    dateLabel.opacity = 1
                    hideDateTimer.start()
                }
            }
        },
        State {
            name: "showCellularNetworkName"
            PropertyChanges {
                target: dateLabel
                opacity: 0
            }
        }
    ]
}
