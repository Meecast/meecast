omweathertest.cpp:  omweathertest.h
	cxxtestgen.pl --error-printer -o omweathertest.cpp omweathertest.h 

omweathertest:  omweathertest.cpp
	g++ -o omweathertest omweathertest.cpp -pthread -I/usr/include/libxml++-2.6 -I/usr/lib64/libxml++-2.6/include -I/usr/include/libxml2 -I/usr/include/glibmm-2.4 -I/usr/lib64/glibmm-2.4/include -I/usr/include/sigc++-2.0 -I/usr/lib64/sigc++-2.0/include -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include \
		-pthread -lxml++-2.6 -lxml2 -lglibmm-2.4 -lgobject-2.0 -lsigc-2.0 -lgthread-2.0 -lrt -lglib-2.0 -L ../core -lomweather-core

all:    omweathertest
	./omweathertest

clean:
	rm *.o omweathertest.cpp omweathertest
