QMAKE_TARGET  = meecast
QMAKE         = $(QNX_HOST)/usr/bin/qmake
TARGET        = $(QMAKE_TARGET)


all: Makefile $(QMAKE_TARGET)

clean:
	$(MAKE) -C ./arm/lib -f Makefile sureclean
	$(MAKE) -C ./arm -f Makefile sureclean
	$(MAKE) -C ./x86/lib -f Makefile sureclean	
	$(MAKE) -C ./x86 -f Makefile sureclean	


Makefile: FORCE	
	$(QMAKE) -spec unsupported/blackberry-armv7le-qcc -o arm/lib/Makefile meecast/core/core.pro CONFIG+=device
	$(QMAKE) -spec unsupported/blackberry-x86-qcc -o x86/lib/Makefile meecast/core/core.pro CONFIG+=simulator
	$(QMAKE) -spec unsupported/blackberry-armv7le-qcc -o arm/Makefile meecast/qt-qml/qt-qml.pro CONFIG+=device
	$(QMAKE) -spec unsupported/blackberry-x86-qcc -o x86/Makefile meecast/qt-qml/qt-qml.pro CONFIG+=simulator
#	$(QMAKE) -spec unsupported/blackberry-armv7le-qcc -o arm/Makefile $(QMAKE_TARGET).pro CONFIG+=device
#	$(QMAKE) -spec unsupported/blackberry-x86-qcc -o x86/Makefile $(QMAKE_TARGET).pro CONFIG+=simulator
#	$(MAKE) -C ./translations -f Makefile update release

FORCE:

$(QMAKE_TARGET): device simulator

device:
	$(MAKE) -C ./arm/lib -f Makefile all
	$(MAKE) -C ./arm -f Makefile all

Device-Debug: Makefile
	$(MAKE) -C ./arm/lib -f Makefile debug
	$(MAKE) -C ./arm -f Makefile debug
	
Device-Release: Makefile
	$(MAKE) -C ./arm/lib -f Makefile release
	$(MAKE) -C ./arm -f Makefile release

simulator:
	$(MAKE) -C ./x86/lib -f Makefile all
	$(MAKE) -C ./x86 -f Makefile all

Simulator-Debug: Makefile
	$(MAKE) -C ./x86/lib -f Makefile debug
	$(MAKE) -C ./x86 -f Makefile debug

