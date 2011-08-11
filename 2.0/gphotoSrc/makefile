all:
	gcc -c -g config.c -o config.o -lgphoto2
	gcc -c -g context.c -o context.o -lgphoto2
	gcc -c -g sample-capture.c -o sample-capture.o -lgphoto2
	ar rcs libSampleCapture.a config.o context.o sample-capture.o
	gcc -c -g -fPIC config.c -o config.o -lgphoto2
	gcc -c -g -fPIC	context.c -o context.o -lgphoto2
	gcc -c -g -fPIC sample-capture.c -o sample-capture.o -lgphoto2
	gcc -shared -W1,-soname,libSampleCapture.so.1 -o libSampleCapture.so.1.0.1 config.o context.o sample-capture.o
	g++ usb.cpp -o usbSnap -L. -lSampleCapture -lgphoto2
clean:
	rm -f *.o *.a *.so.* *.jpg