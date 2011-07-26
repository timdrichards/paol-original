CXX=g++
FLAGS=`pkg-config --cflags --libs opencv` \
	-L /usr/lib/ \
	-lboost_thread \
	-lboost_filesystem-mt \
	-lboost_regex \
	-g

OBJ1=paolMat.cpp buffer.cpp module.cpp main.cpp #consumer.o producer.o writeToDisk.o \
	process.o whiteBoardProcess.o gige.o locateProf.o main.o

all: everything

everything: $(OBJ1)
	      $(CXX) -g -ggdb -Wall -W -o paol-threads $(OBJ1) $(FLAGS)

clean:
	rm -f paol-threads *.o

clean-all:
	rm -f paol-threads *.o *~ *#	      
