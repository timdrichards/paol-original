CXX=g++
FLAGS=-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread \
	-lboost_filesystem-mt \
	-lboost_regex
OBJ1=main.o buffer.o consumer.o producer.o writeToDisk.o \
	process.o whiteBoardProcess.o gige.o paolMat.o



all: everything

everything: $(OBJ1)
	      $(CXX) -g -Wall -W -o paol-threads $(OBJ1) $(FLAGS)

clean:
	rm -f paol-threads *.o

clean-all:
	rm -f paol-threads *.o *~ *#	      
