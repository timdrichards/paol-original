CXX=g++
FLAGS=`pkg-config --cflags --libs opencv` \
	-L /usr/lib/ \
	-lboost_thread \
	-lboost_filesystem-mt \
	-lboost_regex \
	-L/usr/local/lib \
	-lgphoto2 \
	-lm \
	-g
#-lgphoto2_port \

OBJ1=paolMat.cpp buffer.cpp module.cpp whiteBoard.cpp genericProcess.cpp locateSpeaker.cpp ac.cpp lectVideoFrameCreate.cpp computerDistribute.cpp computerProcess.cpp epiphanCapture.cpp WebCamCapture.cpp usbCamC.c usbCam.cpp main.cpp #consumer.o producer.o writeToDisk.o \
	process.o whiteBoardProcess.o gige.o locateProf.o main.o

all: everything

everything: $(OBJ1)
	      $(CXX) -g -ggdb -Wall -W -o paol-threads $(OBJ1) $(FLAGS)

clean:
	rm -f paol-threads *.o outMedia/*

clean-all:
	rm -f paol-threads *.o *~ *#	      

andclean: everything
	rm -f outMedia/*