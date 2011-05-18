all:
	g++ -g -Wall main.cpp -o paol-threaded \
	-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread 	