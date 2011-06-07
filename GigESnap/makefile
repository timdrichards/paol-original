#CXX=g++
#FLAGS=-I /usr/local/include/opencv/ \
	-L/user/local/lib/ \
	-lopencv_core -lopencv_highgui \
	-L /usr/lib/ \
	-lboost_thread \
	-lboost_filesystem-mt \
	-lboost_regex
#OBJ1=Snape.o
#all: everything
#everything: $(OBJ1)
#	      $(CXX) -g -Wall -W -o snap $(OBJ1) $(FLAGS)
#clean:
#	rm -f snape *.o
#clean-all:
#	rm -f snap *.o *~ *#	      

include ARCH


## Executable
EXE	= Snap

$(OBJ_DIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $(VERSION) -c $< -o $@

clean: 
	rm $(EXE)

sample : $(EXE).cpp
	$(CC) $(RPATH) $(TARGET) $(CFLAGS) $(EXE).cpp -o $(EXE) $(SOLIB) $(PVLIB) $(IMLIB)

sample-static : $(EXE).cpp
	$(CC) $(RPATH) $(TARGET) $(CFLAGS) $(EXE).cpp $(SALIB) -o $(EXE) $(SOLIB) $(IMLIB)	
install:
	cp -f $(EXE) $(BIN_DIR)
