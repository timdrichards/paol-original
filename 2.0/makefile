CXX=g++
FLAGS=`pkg-config --cflags --libs opencv` \
	-L /usr/lib/ \
	-lboost_thread \
	-lboost_filesystem-mt \
	-lboost_regex \
	-L/usr/local/lib \
	-lgphoto2 \
	-L. \
	-lSampleCapture \
	-lm

PFLAGS=-g -w

vpath %.o bin/

SOURCES= paolMat.cpp buffer.cpp module.cpp whiteBoard.cpp genericProcess.cpp locateSpeaker.cpp ac.cpp lectVideoFrameCreate.cpp computerDistribute.cpp computerProcess.cpp epiphanCapture.cpp WebCamCapture.cpp usbCam.cpp main.cpp

LIBS= libSampleCapture.a libSampleCapture.so.1.0.1

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=paol-threads

all: $(LIBS) $(SOURCES) $(EXECUTABLE) 

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(FLAGS)

.cpp.o:
	$(CXX) $(PFLAGS) -c $< -o bin/$@ $(FLAGS)

libSampleCapture.a libSampleCapture.so.1.0.1:
	$(MAKE) -C gphotoSrc
	cp gphotoSrc/libSampleCapture.so.1.0.1 .
	cp gphotoSrc/libSampleCapture.a .

clean:
	rm -f paol-threads *.o outMedia/* *.a *.so.*

clean-all:
	rm -f paol-threads *.o *~ *#	      

andclean: everything
	rm -f outMedia/*