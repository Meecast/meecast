import QtQuick 2.2
import Sailfish.Silica 1.0
import "jsUtils.js" as Utils

Page {
    id: about
    allowedOrientations: Utils.decideOrientation();
    property int margin: Theme.paddingSmall

    Rectangle {
        anchors.fill: parent
        color: {
            if (Config.transparency){
                return "transparent"
            }else{
                if (Theme.primaryColor == "#000000"){
                    return  Theme.rgba(Theme.highlightBackgroundColor, Theme.highlightBackgroundOpacity)
                }else{
                    return "black"
                }
            }
        }
    }
    Rectangle{
        anchors.fill: parent
        anchors.topMargin: 80
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        color: "transparent"

        Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            color: "transparent"
        }
        Loader {
            id: background
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            height: 610
            sourceComponent: Image {source: Config.imagespath + "/mask_background_grid.png"}
        }
        Rectangle {
            anchors.top: background.bottom
            width: parent.width
            height: parent.height - 610
            color: "transparent"
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        anchors.topMargin: Theme.paddingLarge
        //contentHeight: column.height
        contentHeight: Theme.paddingLarge*20 + pageheader.height + 
                       icon.height + versiontext.height + abouttext.height + aboutfulltext.height +
                       splitter.height + projectadministratortext.height + prohectadministatorfulltext.height +
                       leadprogrammertext.height + leadprogrammerfulltext.height + localizationtext.height +
                       localizationfulltext.height + leaddesignertext.height + leaddesignerfulltext.height +
                       projectmanagertext.height + projectmanagerfulltext.height + translatorstext.height +
                       translatorsfulltext.height + iconsetext.height + iconsetfulltext.height +
                       licensetext.height + licensefulltext.height + 
                       sourcetext.height + sourcefulltext.height + icon.anchors.topMargin + versiontext.anchors.topMargin + abouttext.anchors.topMargin
        contentWidth: parent.width
        VerticalScrollDecorator {}

        
        Column {
            anchors.fill: parent
            spacing: Theme.paddingLarge
            id: column
            PageHeader {
                id: pageheader
                title: Config.tr("MeeCast for AuroraOS")
            }

            Image {
                id: icon
                source: Config.iconspath + "/" + Config.iconset + "/28.png"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 100
                height: Theme.iconSizeExtraLarge
                width: Theme.iconSizeExtraLarge
                smooth: true
            }
            Label {
                id: versiontext
                text: "Version" + " " + Config.version
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 5
                font.pixelSize: Theme.fontSizeMedium
            }
            Label {
                id: abouttext
                text: Config.tr("About") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin 
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: aboutfulltext
                text: Config.tr("MeeCast - multiplatform highly customizable open source weather forecast client based on OMWeather code. Copyright (C) 2006-2025")
                anchors.leftMargin: 20
                anchors.left: parent.left
                wrapMode: Text.Wrap
                width: parent.width - Theme.horizontalPageMargin 
            }
            /*
            Label {
                id: projectsitetext
                text: Config.tr("Support") + ":"
                anchors.top: aboutfulltext.bottom
                anchors.topMargin: 30
                anchors.leftMargin: 20
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                signal clicked
                id: projectfulltext
                text: "support@meecast.org"
                anchors.leftMargin: 20
                anchors.left: parent.left
                anchors.top: projectsitetext.bottom
                MouseArea {
                    id: mouseArea_web_support
                    anchors.fill: parent
                    onReleased: {
                        Config.showwebsupport();
                    }
                }
            }
            */
    /*
    Image {
            signal clicked
            id: donate_button
    source: Config.imagespath + "/btn_donate_LG.png"
    anchors.top: projectsitetext.bottom
    anchors.rightMargin: 20
    anchors.right: parent.right
            smooth: true	
            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onReleased: {
                    Config.showwebdonation();
                }
            }
        }

        */
            Rectangle {
                id: splitter
                color: "#999999"
                x: 20; width: parent.width - 40; height: 2
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.topMargin: Theme.paddingLarge 
            }
            Label {
                id: projectadministratortext
                text: Config.tr("Project administrator") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: prohectadministatorfulltext
                text: "Vlad Vasilyeu"
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
            }
            Label {
                id: leadprogrammertext
                text: Config.tr ("Programmer") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: leadprogrammerfulltext
                text: "Vlad Vasilyeu"
                anchors.leftMargin: Theme.horizontalPageMargin 
                anchors.left: parent.left
            }
            Label {
                id: localizationtext
                text: Config.tr ("Localization platform") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: localizationfulltext
                text: "http://transifex.net"
                anchors.leftMargin: Theme.horizontalPageMargin 
                anchors.left: parent.left
            }
            Label {
                id: leaddesignertext
                text: Config.tr("Lead designers") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin 
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: leaddesignerfulltext
                text: "Andrew Zhilin, Stephan Beyerle"
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
            }
            Label {
                id: projectmanagertext
                text: Config.tr("Project manager") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: projectmanagerfulltext
                text: "Ludmila Lisovskaya"
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
            }
            Label {
                id: coadministratortext
                text: Config.tr("Co-administrator and contributor") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: coadministratorfulltext
                text: "olf (Olf0)"
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
            }

            Label {
                id: translatorstext
                text: Config.tr("Translators") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: translatorsfulltext
                text: Config.tr("\
    French -  Nicolas Granziano, \n \
    \tPhilippe, Benot Knecht, \n \
    \tQuentin Caillard, \n \
    \tJordi\n") + Config.tr("\
    Russian - Pavel Fialko, Vlad Vasilyeu, \n \
    \tEd Bartosh\n") + Config.tr("\
    Finnish - Marko Vertainen,\n\
    \tdeadhead (A-P Partonen),\n \
    \tjarliim\n") + Config.tr("\
    German -  Claudius Henrichs,\n \
    \tJens Klingen,\n \
    \tDeadHorseRiding\n ") + Config.tr("\
    Italian - Lorenzo Facca,\n \
    \tPavel Fialko, Alessandro Pasotti,\n \
    \tSalvatore Fiorenzo, Ilir Gjika,\n \
    \tLuca Onnis\n") + Config.tr("\
    Spanish - Alejandro Lopez, Agustí Clara\n") + Config.tr("\
    Polish - Tomasz Dominikowski\n") + Config.tr("\
    Slovak - Roman Moravčík\n") + Config.tr("\
    Dutch - Tom Leenders, Michiel Pater\n \
    \tHeimen Stoffels, Nathan Follens\n") + Config.tr("\
    Bulgarian - Zlatko Enikov\n") + Config.tr("\
    Albanian - Ilir Gjika\n") + Config.tr("\
    Chinese - Wong Man Kong, Michael,\n \
    \tgwmgdemj\n") + Config.tr("\
    Portuguese - Jose Narciso,\n") + Config.tr("\
    Turkish - Burak Kaan Kose,\n \
    \tMehmet Çilingir\n") + Config.tr("\
    Catalan - Agustí Clara\n") + Config.tr("\
    Galician - pakoR\n") + Config.tr("\
    Arabic - Tarad Alfandi\n") + Config.tr("\
    Danish - Peter Jespersen\n") + Config.tr("\
    Serbian - Aleksandar Zaric\n") + Config.tr("\
    Norwegian - Peer-Atle Motland\n") + Config.tr("\
    Hungarian - Salmi, László Kara\n") + Config.tr("\
    Slovenian - Boštjan Štrumbelj\n") + Config.tr("\
    Czech - Jakub Kožíšek, Pavel Šrubař\n") + Config.tr("\
    Chinese(Taiwan) - osk2, \n") + Config.tr("\
    Vietnamese - Hieu Nguyen\n \
    \tAnh Dương and truongnx, \n") + Config.tr("\
    Swedish - Angela Nennebrandt,\n \
    \tÅke Engelbrektson, \n \
    \tNiklas Åkerström, \n \
    \tTomislav Veleckovik")
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
            }
            Label {
                id: iconsetext
                text: Config.tr("Iconsets") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
           }
           Label {
                id: iconsetfulltext
    text: "    Beginning - \n\
    Beginning beginning_somehow(at)yahoo.com \n\n\
    Contour - \n\
    Glance - \n\
    MeeCast - \n\
    Andrew Zhilin http://tabletui.wordpress.com \n\n\
    Epona - \n\
    Gianni Polito aka Epona http://crystalxp.net \n\n\
    Grzankas -\n\
    Wojciech Grzanka http://http://grzanka.pl/ \n\n\
    SimpelMee - \n\
    Kim Venetvirta tardolus(at)ovi.com \n\n\
    Noun - \n\
    designed by Adam Whitcroft \n\
    from The Noun Project. http://thenounproject.com\n\n\
    Shiny - \n\
    JyriK http://jyrik.deviantart.com \n\
    jyrik(at)koti.luukku.com";
            anchors.leftMargin: Theme.horizontalPageMargin
            anchors.left: parent.left
            font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                id: licensetext
                text: Config.tr("License") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
            }
            Label {
                id: licensefulltext
    text: "     " + Config.tr ("This software is free software; you can\n \
    redistribute it and/or modify it under \n \
    the terms of the GNU General Public\n \
    License as published by the Free Software \n \
    Foundation; either version 2.1 of the \n \
    License, or (at your option) any \n \
    later version.");
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                font.pixelSize: Theme.fontSizeSmall
            }
            Label {
                id: sourcetext
                text: Config.tr("Source code") + ":"
                anchors.topMargin: Theme.paddingLarge
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                color: "#999999"
                }
            Label {
                id: sourcefulltext
                text: "     " + Config.tr ("http://github.com/Meecast/meecast");
                anchors.leftMargin: Theme.horizontalPageMargin
                anchors.left: parent.left
                font.pixelSize: Theme.fontSizeSmall
            }
        }
    }
}
