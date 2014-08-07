clutter2hildonanimation: weather-main.o weather-clutter.o
	gcc -g -o clutter2hildonanimation weather-clutter.o weather-main.o -I/usr/include -I/usr/X11R6/include `pkg-config --cflags glib-2.0 gconf-2.0 gthread-2.0 hildon-1 clutter-x11-0.8 gtk+-2.0` -pthread -L/usr/X11R6/lib -lm `pkg-config --libs glib-2.0 gthread-2.0 hildon-1 clutter-x11-0.8 gtk+-2.0`
	
weather-clutter.o: weather-clutter.c
	gcc -g -c -o weather-clutter.o -I/usr/include -I/usr/X11R6/include `pkg-config --cflags glib-2.0 gconf-2.0 gthread-2.0 hildon-1 clutter-x11-0.8 gtk+-2.0` -pthread -L/usr/X11R6/lib -lm `pkg-config --libs glib-2.0 gthread-2.0 hildon-1 clutter-x11-0.8 gtk+-2.0`  weather-clutter.c

weather-main.o: weather-main.c
	gcc -g -c -o weather-main.o -I/usr/include -I/usr/X11R6/include `pkg-config --cflags glib-2.0 gconf-2.0 gthread-2.0 hildon-1 clutter-x11-0.8 gtk+-2.0` -pthread -L/usr/X11R6/lib -lm `pkg-config --libs glib-2.0 gthread-2.0 hildon-1 clutter-x11-0.8 gtk+-2.0`  weather-main.c

clean:
	rm -f *.o
	rm -f clutter2hildonanimation
