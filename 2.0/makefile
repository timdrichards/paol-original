CXX=g++-4.4
FLAGS=`pkg-config --cflags --libs opencv` \
	-L /usr/lib/ \
	-lboost_thread \
	-lboost_filesystem-mt \
	-lboost_regex \
	-L/usr/local/lib \
	-lgphoto2 \
	-L. \
	-lSampleCapture \
	-lpthread \
	-lrt \
	-lm \
	-lc 




PFLAGS=-g -w

PVAPIPATH = -Wl,--rpath -Wl,./ 

PVCFLAGS = -fno-strict-aliasing -fexceptions -I/usr/include -Wall -I/home/diw08/code/paolThreads/2.0/PvApi -D_REENTRANT -D_x64 -D_LINUX
PVFFLAGS = -lPvAPI -Bdynamic /usr/local/include/libImagelib.a -Bdynamic -ltiff

vpath %.o bin/

SOURCES= paolMat.cpp buffer.cpp module.cpp whiteBoard.cpp genericProcess.cpp locateSpeaker.cpp ac.cpp lectVideoFrameCreate.cpp computerDistribute.cpp computerProcess.cpp epiphanCapture.cpp WebCamCapture.cpp usbCam.cpp gigE.cpp main.cpp

LIBS= libSampleCapture.a libSampleCapture.so.1.0.1

OBJECTS=$(SOURCES:.cpp=.o)

HEADERS=$(SOURCES:.cpp=.o)

EXECUTABLE=paol-threads

all: $(LIBS) $(SOURCES) $(HEADERS) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(PFLAGS) $(PVAPIPATH) $(PVCFLAGS) $^ -o $@ $(FLAGS) $(PVFFLAGS)

gigE.o:
	$(CXX) $(PFLAGS) $(PVAPIPATH) $(PVCFLAGS) -c gigE.cpp -o bin/gigE.o $(FLAGS) $(PVFFLAGS)

main.o:
	$(CXX) $(PFLAGS) $(PVAPIPATH) $(PVCFLAGS) -c main.cpp -o bin/main.o $(FLAGS) $(PVFFLAGS)

.cpp.o:
	$(CXX) $(PFLAGS) -c $< -o bin/$@ $(FLAGS)

libSampleCapture.a libSampleCapture.so.1.0.1:
	$(MAKE) -C gphotoSrc
	cp gphotoSrc/libSampleCapture.so.1.0.1 .
	cp gphotoSrc/libSampleCapture.a .

clean:
	rm -f paol-threads *.o outMedia/* *.a *.so.* bin/*

clean-all:
	rm -f paol-threads *.o *~ *#	      

andclean: everything
	rm -f outMedia/*