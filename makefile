all:
	g++ -g -Wall -Wall -c buffer.cpp -o buffer.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -Wall -c consumer.cpp -o consumer.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -Wall -c producer.cpp -o producer.o \
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
	g++ -g -Wall -W main.o buffer.o consumer.o producer.o -o paol-threads \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread

buffer:
	g++ -g -Wall -Wall -c buffer.cpp -o buffer.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -W main.o buffer.o consumer.o -o paol-threads \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread

consumer:
	g++ -g -Wall -Wall -c consumer.cpp -o consumer.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -Wall -c main.cpp -o main.o \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread
	g++ -g -Wall -W main.o buffer.o consumer.o -o paol-threads \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread

clean:
	rm paol-threaded *.o