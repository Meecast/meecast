VERSION = 0.5.9
TEMPLATE = subdirs
CONFIG += ordered

lupdate_inclusion {
    SOURCES += ../assets/*.qml
}

device {
	CONFIG(release, debug|release) {
		DESTDIR = o.le-v7
	}
	CONFIG(debug, debug|release) {
		DESTDIR = o.le-v7-g
	}
}

simulator {
	CONFIG(release, debug|release) {
		DESTDIR = o
	}
	CONFIG(debug, debug|release) {
		DESTDIR = o-g
	}
}

OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

suredelete.target = sureclean
suredelete.commands = $(DEL_FILE) $${MOC_DIR}/*; $(DEL_FILE) $${RCC_DIR}/*; $(DEL_FILE) $${UI_DIR}/*
suredelete.depends = distclean

QMAKE_EXTRA_TARGETS += suredelete


#SUBDIRS += meecast omweather-weather-com-stations-db omweather-gismeteo-ru-stations-db omweather-yr-no-stations-db omweather-foreca-com-stations-db omweather-bom-gov-au-stations-db omweather-hko-gov-hk-stations-db
SUBDIRS += meecast omweather-foreca-com-stations-db 


