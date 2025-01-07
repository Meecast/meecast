%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

%define _binary_payload w2.xzdio

%define vrsn 1.11.17
%define rlse 1

Name:       harbour-meecast
Summary:    MeeCast for SailfishOS
# Versioning for all (sub-)packages adheres to Semantic Versioning 2.0.0, see https://semver.org/
# Because the Sailfish-OBS sets the version of all subpackages to the one of the primary package,
# this is already done here, hence applies to MeeCast built by any build-system (for consistency
# and to avoid confusing people and RPM).
Version:    %{vrsn}
Release:    %{rlse}
Group:      Utility
License:    LGPL-2.1-only
URL:        https://github.com/Meecast/meecast
Source0:    %{url}/archive/%{version}/%{name}-%{version}.tar.gz
#Temporary
#Requires:       libmeegotouch-devel
BuildRequires:  pkgconfig(sailfishapp)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(qdeclarative5-boostable)
BuildRequires:  desktop-file-utils
BuildRequires:  pkgconfig(libcurl)
BuildRequires:  pkgconfig(zlib)
BuildRequires:  pkgconfig(sqlite3)
#BuildRequires:  pkgconfig(gconf-2.0)
BuildRequires:  pkgconfig(dbus-glib-1)
BuildRequires:  pkgconfig(libxml-2.0)
BuildRequires:  pkgconfig(Qt5Xml)
BuildRequires:  pkgconfig(Qt5XmlPatterns)
#BuildRequires:  libxml2-devel
BuildRequires:  gettext
BuildRequires:  pkgconfig(dconf)
BuildRequires:  libkeepalive-devel
BuildRequires:  mlite-qt5-devel
BuildRequires:  pkgconfig(Qt5Concurrent)
#BuildRequires:  libqt-devel
BuildRequires:  qt5-qtpositioning-devel
BuildRequires:  qml(Sailfish.Silica)
#Requires:      qt5-qtpositioning
#Requires:      sailfishsilica-qt5
#Requires:      qt5-qtdeclarative-import-models2
#Requires:      zlib

# This %%description section includes metadata for SailfishOS:Chum, see
# https://github.com/sailfishos-chum/main/blob/main/Metadata.md
%description
MeeCast is a multiplatform, highly customizable, open source, weather forecast client based on OMWeather code.
%if 0%{?_chum}
PackageName: MeeCast
Type: desktop-application
Categories:
 - Network
 - Science
 - Utility
 - News
 - DataVisualization
 - Qt
DeveloperName: Vasvlad, OMWeather team and contributors
Custom:
   Repo: %{url}/tree/sailfishos
PackageIcon: %{url}/raw/sailfishos/meecast/core/data/desktop/Icon/meecast.svg
Screenshots:
  - https://openrepos.net/sites/default/files/packages/13090/screenshot-sailfishosemulator2.png
  - https://openrepos.net/sites/default/files/packages/13090/screenshot-sailfishosemulator.png
  - https://openrepos.net/sites/default/files/packages/13090/screenshot-cover.png
Links:
  Homepage: https://openrepos.net/content/vasvlad/meecast-sailfishos
  Help: https://openrepos.net/content/vasvlad/meecast-sailfishos#comments
  Bugtracker: %{url}/issues
%endif


%package daemon
Version: %{vrsn}
Release: %{rlse}
Summary: MeeCast background daemon for SailfishOS
Group:    Utility
License:  LGPL-2.1-only
Requires: %{name}
Requires: systemd
BuildRequires:  pkgconfig(contentaction5)

%description daemon
MeeCast daemon obtains weather data in the background.
%if 0%{?_chum}
PackageName: MeeCast daemon
Type: desktop-application
Categories:
 - Network
 - Science
 - Utility
 - News
 - DataVisualization
 - Qt
DeveloperName: Vasvlad, OMWeather team and contributors
Custom:
   Repo: %{url}/tree/sailfishos
PackageIcon: %{url}/raw/sailfishos/meecast/core/data/desktop/Icon/meecast.svg
Screenshots:
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20150114135243.png
Links:
  Homepage: https://openrepos.net/content/vasvlad/meecast-daemon
  Help: https://openrepos.net/content/vasvlad/meecast-daemon#comments
  Bugtracker: %{url}/issues
%endif


%package lockscreen
Version: %{vrsn}
Release: %{rlse}
Summary: MeeCast widget for SailfishOS' lockscreen
Group:    Utility
License:  LGPL-2.1-only
Requires: %{name}
Requires: %{name}-daemon >= 0.3
Requires: patchmanager
Requires: systemd

%description lockscreen
MeeCast's lockscreen widget displays weather information on SailfishOS' lockscreen.
%if 0%{?_chum}
PackageName: MeeCast lockscreen widget
Type: desktop-application
Categories:
 - Network
 - Science
 - Utility
 - News
 - DataVisualization
 - Qt
 - Applet
DeveloperName: Vasvlad, OMWeather team and contributors
Custom:
   Repo: %{url}/tree/sailfishos
PackageIcon: %{url}/raw/sailfishos/meecast/core/data/desktop/Icon/meecast.svg
Screenshots:
  - https://openrepos.net/sites/default/files/packages/678/screenshot-20150201162807.png
Links:
  Homepage: https://openrepos.net/content/vasvlad/meecast-lockscreen
  Help: https://openrepos.net/content/vasvlad/meecast-lockscreen#comments
  Bugtracker: %{url}/issues
%endif


# Detect building for SailfishOS >= 4.6.0 or build subpackage eventview any way.
# %%{sailfishos_version} seems to be undefined in the Sailfish-SDK, but
# is defined for the SailfishOS-OBS when using a SFOS DoD-repo, see e.g.
# https://build.sailfishos.org/project/prjconf/sailfishos:4.6
%if %{undefined sailfishos_version} || 0%{?sailfishos_version} >= 40600
%package eventview
Version: %{vrsn}
Release: %{rlse}
Summary: MeeCast widget for SailfishOS' eventsview
Group:    Utility
License:  LGPL-2.1-only
Requires: %{name}
Requires: %{name}-daemon >= 0.9
# Require Lipstick Weather Widget, which only exists as a separate package on
# SailfishOS >= 4.6.0, see https://forum.sailfishos.org/t/18678/5
Requires: lipstick-jolla-home-qt5-weather-widget-settings
# Require these to be able to set a dconf key of the primary user in %%post and %%preun scriplets
Requires: coreutils
Requires: systemd
Requires: dconf
# Provide, obsolete and conflict with the event subpackage for SailfishOS < 4.6.0
Provides: %{name}-event
Obsoletes: %{name}-event
Conflicts: %{name}-event

%description eventview
MeeCast eventview widget displays weather information at the top of SailfishOS' eventsview.
%if 0%{?_chum}
PackageName: MeeCast eventsview widget for SailfishOS >= 4.6.0
Type: desktop-application
Categories:
 - Network
 - Science
 - Utility
 - News
 - DataVisualization
 - Qt
 - Applet
DeveloperName: Vasvlad, OMWeather team and contributors
Custom:
   Repo: %{url}/tree/sailfishos
PackageIcon: %{url}/raw/sailfishos/meecast/core/data/desktop/Icon/meecast.svg
Screenshots:
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20151006090340.png
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20151011183657.png
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20151014084258.png
Links:
  Homepage: https://openrepos.net/content/vasvlad/meecast-event-view
  Help: https://openrepos.net/content/vasvlad/meecast-event-view#comments
  Bugtracker: %{url}/issues
%endif
%endif


# Detect building for SailfishOS < 4.6.0 or build subpackage event any way.
%if 0%{?sailfishos_version} < 40600
%package event
Version: %{vrsn}
Release: %{rlse}
Summary: MeeCast widget for SailfishOS' eventsview
Group:    Utility
License:  LGPL-2.1-only
Requires: %{name}
Requires: %{name}-daemon >= 0.9
Requires: sailfish-version < 4.6.0

%description event
MeeCast eventview widget displays weather information at the top of SailfishOS' eventsview.
%if 0%{?_chum}
PackageName: MeeCast eventsview widget for SailfishOS < 4.6.0
Type: desktop-application
Categories:
 - Network
 - Science
 - Utility
 - News
 - DataVisualization
 - Qt
 - Applet
DeveloperName: Vasvlad, OMWeather team and contributors
Custom:
   Repo: %{url}/tree/sailfishos
PackageIcon: %{url}/raw/sailfishos/meecast/core/data/desktop/Icon/meecast.svg
Screenshots:
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20151006090340.png
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20151011183657.png
  - https://openrepos.net/sites/default/files/packages/13234/screenshot-20151014084258.png
Links:
  Homepage: https://openrepos.net/content/vasvlad/meecast-event-view
  Help: https://openrepos.net/content/vasvlad/meecast-event-view#comments
  Bugtracker: %{url}/issues
%endif
%endif


%prep
%setup -q

%build
%qtc_qmake5 meecast.pro
%qtc_make %{?_smp_mflags}

%install
%qmake5_install
#make INSTALL_ROOT=%{buildroot} install
#rm %{buildroot}/opt/com.meecast.omweather/lib/libomweather-core.a
desktop-file-install --delete-original  \
   --dir %{buildroot}%{_datadir}/applications  \
   %{buildroot}%{_datadir}/applications/*.desktop


%post daemon
pkill -x meecastd
systemctl-user enable meecastd.service
systemctl-user start meecastd.service
exit 0

%preun daemon
# Removal
if [ "$1" = "0" ]; then
    systemctl-user disable meecastd.service
    systemctl-user stop meecastd.service
fi
#pkill -x meecastd
#systemctl-user daemon-reload
exit 0


%pre lockscreen
if [ -f %{_sbindir}/patchmanager ]; then
   %{_sbindir}/patchmanager -u sailfishos-lockscreen-meecast-patch || true
fi

%post lockscreen
pkill -x meecastd
systemctl-user enable meecastd.service
systemctl-user start meecastd.service
exit 0

%preun lockscreen
if [ -f %{_sbindir}/patchmanager ]; then
   %{_sbindir}/patchmanager -u sailfishos-lockscreen-meecast-patch || true
fi


%if %{undefined sailfishos_version} || 0%{?sailfishos_version} >= 40600
%post eventview
# Activate Lipstick Weather Widget on SFOS >= 4.6.0
su --login "$(loginctl --no-legend list-sessions | grep -F seat0 | tr -s ' ' | cut -f 4 -d ' ')" --command='dconf write /desktop/lipstick-jolla-home/force_weather_loading true' || true

%preun eventview
# Removal:
if [ "$1" = "0" ]; then
# See https://forum.sailfishos.org/t/sfos-4-6-foreca-meecast-how-to-re-enable-the-weather-infos-in-events-view/18678/25 :
# su --login "$(loginctl --no-legend list-sessions | grep -F seat0 | tr -s ' ' | cut -f 4 -d ' ')" --command='dconf write /desktop/lipstick-jolla-home/force_weather_loading false' || true
   su --login "$(loginctl --no-legend list-sessions | grep -F seat0 | tr -s ' ' | cut -f 4 -d ' ')" --command='dconf reset /desktop/lipstick-jolla-home/force_weather_loading' || true
fi
%endif


%files
%defattr(-,root,root,-)
%{_datadir}/applications/%{name}.desktop
%{_bindir}/%{name}
/usr/share/%{name}
#%%{_datadir}/iconsets
%{_datadir}/icons/hicolor
#/opt/com.meecast.omweather/share

%files daemon
%defattr(-,root,root,-)
%{_bindir}/meecastd
%{_bindir}/meecast_predaemon
%{_userunitdir}/meecastd.service
%{_libdir}/qt5/qml/org/meecast/data

%files lockscreen
%defattr(-,root,root,-)
%{_datadir}/patchmanager/patches/sailfishos-lockscreen-meecast-patch

%if %{undefined sailfishos_version} || 0%{?sailfishos_version} >= 40600
%files eventview
%defattr(-,root,root,-)
#/usr/lib/qt5/qml/Sailfish/Weather
%{_libdir}/qt5/qml/Sailfish/Weather
%endif

%if 0%{?sailfishos_version} < 40600
%files event
%defattr(-,root,root,-)
#/usr/lib/qt5/qml/Sailfish/Weather
%{_libdir}/qt5/qml/Sailfish/Weather
%endif


%changelog
* Wed Jan 01 2025 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.17
- Added icons to gismeteo.ru
- Changed url for weather forecast from gismeteo again

* Sun Dec 23 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.15
- Changed url for weather forecast from gismeteo

* Sun Dec 22 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.14
- Added icons to gismeteo.ru
- Fixed segmentation fault for gismeteo data

* Sat Dec 21 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.13
- Changed url for hourly weather forecast from gismeteo again
- Fixed bug - Since version 1.11.8 of MeeCast, no clickable list for the forecast and Pulley-Menu broken #79

* Fri Dec 21 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.12
- Changed url for hourly weather forecast from gismeteo

* Fri Dec 20 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.11
- Added icons and description to gismeteo.ru
- Fixed some problems in gismeteo.ru data (pressure and etc)
- Added hourly weather forecasts to gismeto source

* Sat Dec 14 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.10
- Added Torzhok to weather.com and openweathermap.org
- Added icons and description to fmi.fi 
- Restored gismeteo.ru source
- Updated Slovak, Slovenian, Swedish, Russian translations
- Removed tending to mix up languages in selecting region of fmi.fi source. Replaced all region's name to Finnish.
- Optimised layout of MeeCast's Event-View #75. Thanks @pagism, @FingusDE and @Olf0

* Sun Nov 24 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.8
- Added icons and description to fmi.fi 
- Updated Norwegian translation
- Fixed segmentation fault
- Fixed hour's negative temperature problem in fmi.fi source

* Sun Nov 03 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.7
- Fixed a segmentation fault when the configuration file is in read-only mode
- Added icons and descriptions to fmi.fi
- Fixed problem with symbols '=' and '?' in station file name
- Updated Russian, Slovenian translations
- Fixed the process of changing language setting

* Tue Oct 08 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.6
- Updated Russian and Swedish translations
- Added Location to Permissions [X-Sailjail] section
- Fixed warnings in Sailfish Harbour rpm validator
- Improved in UI on About page

* Sat Oct 05 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.5
- Added icons and description to fmi.fi
- Added to [X-Sailjail] Permissions=Internet, removed Sandboxing=Disabled

* Thu Sep 26 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.4
- Nothing

* Thu Sep 26 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.3
- Improved in settings UI

* Mon Sep 16 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.2
- Fixed sunrise and sunset for fmi.fi source

* Thu Sep 12 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.11.1
- Fixed problem Segmentation fault while trying to load fmi.fi stations, see issue #62

* Wed Sep 11 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.40-3
- Overhaul spec file, see PR #57

* Thu Jul 18 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.40
- Fixed segmentation fault for weather.com
- A build fix (999d41f) closes issue #56
- Another build fix (6ae896b) closes issue #58

* Thu Jun 13 2024 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.39-2
- Adapted to SailfishOS 4.6
- Fixed license

* Sun Nov 12 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.39
- Fixed deamon for updating yr.no

* Sun Nov 12 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.38
- Added station Giulianova in Italy to weather.com, yr.no. Fixed Teramo gps-coordinates #45
- Fixed problem hko.gov.hk source

* Sun Oct 15 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.37
- Fixed problem with yr.no stations. The reason is incorrect work with locales #44

* Sun Oct 08 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.36
- Fixed problem with drawing wind direction
- Restored source yr.no

* Sat Sep 23 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.35
- Updated Slovak translation. Thanks Ladislav Hodas aka holasl
- Fixed - Bom.gov.au as weather source shows the previous day. #41 (@vasvlad)

* Wed Sep 06 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.34
- The new version of packages: daemon, lockscreen, eventview
- Trying to fix segmentation fault in openweathermap.org source
- Trying to fix segmentation fault in weather.com source

* Thu Aug 31 2023 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.33
- Fixed for SailfishOS 4. Many thanks for @carmenfdezb

* Sat Nov 07 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.32
- Added sattion Manqutay (Iran) to weather.com source

* Sat Aug 29 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.31
- Fixed 'About' page

* Tue Aug 25 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.30
- Small fixes

* Sat Aug 22 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.29
- Fixed problem with weather.com source (again)

* Tue Aug 11 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.28
- Fixed problem hours forecast

* Sun Aug 02 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.27
- Fixed problem with weather.com source (again :)

* Sat Jun 06 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.26
- Fixed problem with weather.com source
- Updated Russian, Slovenian, Swedish, Serbian, Chinese translations

* Mon Jan 20 2020 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.25
- Fixed problem with weather.com source
- Updated Germany and Chinese translations

* Tue Dec 24 2019 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.24
- Updated Russian, Slovenian and Swedish translations
- Fixed the New Year bug for weather.com source

* Tue Jun 11 2019 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.23
- Fixed temperature bug in gismeteo source

* Sun May 12 2019 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.22
- Fixed temperature bug in foreca source

* Wed Jan 30 2019 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.21
- Fixed text color on cover and main pages for various themes
- Adapted eventview for Sailfish 3.0.1.11
- Added refreshing for Icon in lock screen

* Sun Nov 11 2018 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.20
- Added station Nonoai in Brazilia to openweathermap.org
- Added icon for openweathermap.org source
- Added string for translation
- Updated Russian, Dutch, Slovenian, Swedish translations
- Changed desktop icon
- Fixed source gismteo.ru

* Sun Jan 07 2018 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.19
- Added string for translation
- Fixed the New Year bug for weather.com and foreca.mobi sources
- Fixed char and icon too small in coverpage in XperiaX

* Sun Oct 15 2017 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.18
- Added string for translation
- Added stations to Ukraine in gismeteo.ru source
- Fixed segmentation fault for Melbourne(Australia) in bom.gov.au source

* Mon Sep 18 2017 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.17
- Added string for translation
- Added Iran and Georgia to foreca.com
- Updated Russian, Dutch, Norwegian, Slovenian, Swedish, Germany translations

* Sat Dec 31 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.16
- Added new translation strings for weather.com
- Added icon for foreca.com
- Disabled Map
- Added Innopolis city to gismeteo.ru, foreca.com sources
- Updated Russian, Danish, Dutch, Polish, Swedish, Slovenian, Galician, Spanish, Catalan translations

* Sun Aug 28 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.15
- Fixed problem with landscape orientation

* Fri Aug 12 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.14
- Fixed problem with landscape orientation

* Wed Aug 10 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.13
- Adapted for big size screen
- Updated Catalan, Spanish, Dutch, Slovenian translations
- Added string for translation

* Fri Mar 25 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.12
- Added icon for gismeteo.ru
- Added string for translation
- Fixed station Sankt-Peterburg in gismeteo.ru source
- Added station Daugavpils to weather.com source

* Sat Mar 12 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.11
- Fixed problem with sunrise and sunset in gismeteo.ru source
- Added icon for gismeteo.ru
- Added new translation string for gismeteo.ru
- Disabled logging

* Sun Mar 06 2016 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.10
- Added string for translation
- Fixed problem with gisemteo.ru source
- Updated Russian, Catalan, Germany, Serbian, Swedish, Spanish, Russian, Danish, Norwegian, Dutch, Slovenian, Czech translations

* Tue Nov 17 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.9
- Added stations in Russia to Openweathermap.org (Sverdlovskaya oblast, Krasnodarkiy Kray and Leningradskaya oblast)
- Updated Russian, Swedish, Finnish, Norwegian tarnslations
- Fixed 12/24 clock viewing problem

* Mon Nov 02 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.8
- Fixed problem with sources list

* Sun Nov 01 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.7
- Added new translation string for gismeteo.ru
- Fixed problem in openweathermap.org source again
- Fixed problem useless *.a files https://github.com/Meecast/meecast/issues/23
- Added desktop icons (108x108 128x128 256x256)
- Added Galician translation
- Updated Russian, Swedish tarnslations


* Sun Oct 18 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.6
- Increased count of days in predeamon (for Landscape mode on EventView)
- Fixed problem in openweathermap.org source

* Sun Aug 16 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.4
- Nothing

* Sun Aug 16 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.3
- Fixed problem with 'Next day' function
- Added new translation string for gismeteo.ru

* Sat Jul 11 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.2
- Added GPS station
- Added icon for openstreetmap.org

* Wed Jun 03 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.1
- Fixed problem with sunrise in fmi source again
- Fixed crashing related zero size of config file
- Fixed problem with incorrect time in hourly forecast in fmi source
- Updated Germany, Danish translations

* Sun Apr 12 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.1.0
- Added eventview widget

* Sun Apr 12 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.0.4
- Fixed problem with sunrise in fmi source
- Updated Catalan, Chinese, Czech, Dutch, Finnish, Germany, Slovenian, Spanish, Swedish, Turkish, Italian translations
- Fixed problem with wind speed in gismeteo.ru source

* Mon Mar 09 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 1.0
- Added Hungarian trnslator László Kara to About page
- Added Åke Engelbrektson as Swedish translator
- Fixed problem with weather.com source (all days show night weather forecast)
- Fixed problem with CoverActions on Cover page after manual start updating of weather forecast
- Fixed Slovenian translator name to Boštjan Štrumbelj
- Rewrote About page
- Fixed problem with incorrect view on the main page(with nil data) when weather forecast is updated

* Thu Mar 05 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.18
- Fixed problem with crash of the MeeCast while updating of weather forecast on cover mode
- Restored Slovenian translation

* Thu Feb 19 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.17
- Optimised png images by OptiPNG and advpng. Thanks Alexander Self-Perfection
- Updated Catalan, Chinese, Czech, Dutch, Finnish, French, Germany, Norwegian, Slovenian, Spanish, Swedish, Italian, Danish translations
- Fixed problem with upgrading meecast daemon
- Fixed problem with 'Undefined' text after first widget start
- Fixed problem with hko source
- Fixed color of text for LPM mode
- Disable semitransparent background for LPM mode
- Fixed problem with updating

* Sun Feb 01 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.16
- Moved a little bit down wallpaper widget
- Fixed problem in daemon with incorrect weather forecast data
- Moved a little bit up and right wallpaper widget
- Added widget for Lockscreen
- Added string for translation from gismeteo.ru source

* Wed Jan 14 2015 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.15
- Added daemon for controlling updating weather forecast and displaying weather on wallpaper
- Added option for controlling lockscreen widget

* Mon Dec 22 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.14
- Added string for translation from openweathermap.org source
- Removed unnecessary second refresh action on cover page. Thanks George Nikandrov
- Set source logo to top of cover page. Thanks George Nikandrov
- Fixed problem with Icon and Null wind speed data on cover page
- Fixed problem with 'Next station' and 'Update' buttons on cover page. Thanks TMavica

* Fri Dec 12 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.13
- Added Durovka to gismeteo.ru and foreca.com sources
- Added India to Openweathermap.org
- Decreased font size in station name on cover page for station name length from 19 to 20
- Fixed problem with Beaufort Wind unit
- Updated Catalan, Dutch, Finnish, Germany, Slovenian, Spanish, Italian, Czech, Swedish, Russian, Danish translations

* Thu Nov 27 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.12
- Fixed problem with hourly forecast view
- Added Palanga(Lithuania) to weather.com, yr.no, openweathermap.org sources
- Removed donation link
- Added wake up internet connection for refresh weather forecast
- Added station Poselok rudnika Kupol to gismeteo.ru source
- Fixed problem with Denmark, Germany stations in openweathermap.org source
- Fixed incorrect name of pressure from "Pa" to "kPa"
- Updated Catalan, Dutch, Finnish, Germany, Norway, Slovenian, Spanish, Italian, Czech, Danish translations

* Tue Oct 28 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.11
- Fixed broken cover (for new firmware)

* Mon Sep 01 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.10
- Added translation string for gismeteo.ru
- Adapted for big size screen
- Updated Slovenian, Italian, Swedish translations
- Fixed issue with auto update

* Tue Aug 26 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.9
- Fixed problem with USA like style time for last update, sunrise, sunset and hourly information
- Added stations Lignano Pineta, Lignano Riviera, Lignano Sabbiadoro (Italy) to weather.com, foreca.com source
- Added stations Lignano Sabbiadoro (Italy) to gismeteo.ru source
- Added all stations from Regione Autonoma Friuli Venezia Giulia to openwearthermap.org source
- Added station Cesena, ER, Italy to weather.com, openweathermap.com source
- Added many stations from Italy to yr.no source
- Changed value for opacity in backgaound on Cover Page to 0.0
- Fixed problem with actions on Cover page
- Fixed problem with function 'Next station' on Cover page
- Updated Finnish, Germany translations

* Thu Aug 14 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.8
- Added Sunrise and sunset for fmi.fi source
- Added Precipitation
- Added Precipitation rate
- Added Precipitation data to hourly page
- Added Dewpoint
- Added dewpoint for weather.com source
- Fixed problem with big icon on Main form
- Fixed problem with a lag after removal station
- Updated Catalan, Spanish, Russian, Dannish translations

* Sun Aug 03 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.7
- Fixed sunrise and sunset time for weather.com source again :)
- Updated Italian, Hungarian, Turkish translations

* Tue Jul 22 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.6
- Fixed donation link
- Added translation string for gismeteo.ru
- Fixed sunrise and sunset time for weather.com source
- Added fmi.fi source
- Updated Catalan, Germany, Slovenian, Spanish, Russian, Hungarian, Danish, Italian, Finnish translations
- Fixed problem with null wind direction on main form
- Added the new source fmi.fi

* Thu Jul 03 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.5
- Fixed problem with Now/Today waether data for Main Page
- Fixed problem with null data for day forecast in weather.com source
- Updated Catalan, Finnish, Norwegian, Slovenian, Danish, Spanish, Italian, Russian, Hungarian, French translations
- Fixed problem with data (humidity, feels like...) in current weather in foreca.com source
- Decreased font for 'Last update" on cover page
- Fixed problem with station name font size on cover page
- Added stations to Finland in yr.no source
- Added hourly weather forecast for yr.no source

* Sun Jun 22 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.4
- Fixed problem with current night forecast
- Fixed memory leaks
- Fixed crashing

* Sun Jun 15 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.3
- Fixed problem with data(not current day) for more then one station
- Added United Kingdom to foreca.com source
- Added Germany's stations to openweathermap.org source

* Wed Jun 11 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.2
- Fixed problem with hours forecast button
- Fixed problem with today data for weather.com source
- Fixed problem with n/a icon for Atmos iconset
- Fixed problem with hourly foreasts
- Added Danish translation
- Updated Catalan, Chinese (Taiwan), Czech, Finnish, German, Italian, Norwegian, Serbian, Slovenian, Spanish, Russian, Hungarian, French translations

* Fri May 02 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.9.1
- Fixed problem with swiping to another station
- Improved design of transparent and black themes
- Added stations to Finland in source yr.no
- Fixed problem with converting map URLs
- Added separator for stations on Main form
- Added icon for openstreetmap.org

* Fri Apr 18 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.18.1
- Fixed problem with current weather in foreca.com source
- Updated Catalan, Serbian, Slovenian, Spanish, Russian translations
- Fixed problems with transparency

* Tue Apr 08 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.17.1
- Added transparency
- Fixed problem with Units view in Menu Settings
- Fixed problem with current weather in foreca.com source
- Changed behaviour for displaying label (Now/Today) on Cover Page
- Updated Catalan, Czech, Finnish, Slovenian, Spanish, German, Italian, Arabic translations

* Sun Mar 30 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.16.1
- Fixed segmentation fault
- Fixed problem with translations
- Fixed problem with logo on cover page
- Fixed centering problem of the graphics on the details page
- Increased icon size on CoverPage
- Reordered items in Main PullMenu
- Updated German and Russian translations
- Changed label text "Update" to "Refresh"

* Wed Mar 26 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.15.1
- Updated Denmark for openstreetmap.org
- Disabled menu item "Adjust GPS station"
- Fixed problem with crashing application with foreca.com data
- Changed order of items in PullMenu
- Fixed font size in station name on Cover Page
- Fixed problem with size of map image
- Updated Catalan, Czech, Finnish, Norwegian, Serbian, Slovenian, Spanish, Turkish, German translations

* Sun Feb 23 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.14.1
- Fixed problem with downloading

* Sat Feb 22 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.13.1
- Fixed problem on Cover page

* Sat Feb 22 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.12.1
- Fixed problem on Cover page

* Sat Feb 22 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.11.1
- Polished Cover page
- Disable unused configuration files

* Tue Feb 18 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.10.1
- Adapted for Harbour (Jolla store)
- Disabled GPS-station

* Sat Feb 15 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.9.1
- Updated Finnish, Norwegian, Arabic, Serbian, Turkish, Dutch, Hungarian translations
- Added Slovenian and Czech translations
- Fixed memory leaks
- Fixed problem with yr.no database for Belarus
- Added icon and descripion string for openweathermap.org source

* Sun Feb 02 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.8.1
- Fixed problem with long text in last update string
- Added possibility to switch off 'Last update' on Cover Page
- Fixed problem in About Page
- Redesigned wind information on Cover Page
- Updated Norwegian, Arabic, Serbian, Turkish, Dutch, Hungarian translations
- Added Slovenian and Czech translations

* Sun Jan 26 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.7.1
- Added Wind speed and Wind direction to Cover Page
- Added text 'Now' to Cover Page
- Polished Cover Page

* Thu Jan 23 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.6.1
- Removed button refresh from main form and add Item menu 'Update' to pulley menu
- Redesigned form for delete location
- Fixed problem with transparency Cover window and font size on Cover Window
- Added last update information on Cover Page
- Added new icons from Stephan Beyerle (Thank you very much Stephan)
- Added background Image to Cover (Thank you very much Stephan again)

* Sun Jan 19 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.5
- Fixed various small problems
- Fixed problem with size of font in CoverPage
- Fixed problem with scroll down for hours in FullWeather page
- Added possibility to refresh weather forecast using the CoverPage
- Fixed problem with open many applications(MeeCast) when you click the icon

* Sat Jan 18 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.4.2
- Fixed problem with refreshing weather forecast

* Fri Jan 17 2014 Vlad Vasilyeu <vasvlad@gmail.com> - 0.8.4.1
- First release for SailfishOS

# Changelog format: https://lists.fedoraproject.org/archives/list/devel@lists.fedoraproject.org/thread/SF4VVE4NBEDQJDJZ4DJ6YW2DTGMWP23E/#6O6DFC6GDOLCU7QC3QJKJ3VCUGAOTD24

