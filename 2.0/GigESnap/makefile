## Executable
EXE = Snap
CC = g++-4.4
PVAPIPATH = -Wl,--rpath -Wl,./ 

PVCFLAGS = -fno-strict-aliasing -fexceptions -I/usr/include -Wall -I./PvApi -D_REENTRANT -D_x64 -D_LINUX
PVSOLIB = -lpthread -lrt -Bdynamic -lm -lc

sample : $(EXE).cpp
	$(CC) $(PVAPIPATH) $(PVCFLAGS) $(EXE).cpp -o $(EXE) $(PVSOLIB) -lPvAPI -Bstatic /usr/local/include/libImagelib.a -Bdynamic -ltiff

