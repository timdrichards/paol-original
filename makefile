all:
	g++ -g -Wall -Wall -c buffer.cpp -o buffer.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -W -c main.cpp -o main.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -W main.o buffer.o -o paol-threads \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread

clean:
	rm paol-threaded *.o