#gcc -g -Wno-write-strings -g -O2 -o camCap cameraCapture.cpp `pkg-config --libs opencv` -I/opt/local/include -L/opt/local/lib -lboost_thread-mt -lboost_program_options -lboost_filesystem -lboost_system
OBJS = cameraCapture.cpp
CXX = gcc
CXXFLAGS = -g -Wno-write-strings -g -O2 
LIBS = `pkg-config --libs opencv` -I/opt/local/include -L/opt/local/lib -lboost_thread-mt -lboost_program_options -lboost_filesystem -lboost_system

camCap : $(OBJS)
	$(CXX) $(CXXFLAGS) -o camCap $(OBJS) $(LIBS)

clean:
	rm camCap


