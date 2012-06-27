SHELL = /bin/sh
top_srcdir = .
srcdir = .


.SUFFIXES:
.SUFFIXES: .cpp .o

OPT= -O3

AR = ar
AR_FLAGS = rc
RANLIB = ranlib

CC = gcc
CFLAGS =  -Wno-write-strings -g -O2
LDFLAGS = -Wno-write-strings 
LIBS = `pkg-config opencv --cflags --libs` -I/opt/local/include -L/opt/local/lib -lboost_thread-mt 
#INSTALL = /usr/bin/install -c
prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
infodir = $(prefix)/info

# Sources
SOURCES=main.cpp computerMain.cpp whiteBoard.cpp WebCamCapture.cpp usbCam.cpp paolMat.cpp module.cpp locateSpeaker.cpp lectVideoFrameCreate.cpp genericProcess.cpp computerProcess.cpp computerDistribute.cpp buffer.cpp ac.cpp 
#gigE.cpp epiphanCapture.cpp
DOCS=
MISC=configure mkinstalldirs install-sh
OBJS=main.o computerMain.o whiteBoard.o WebCamCapture.o usbCam.o paolMat.o module.o locateSpeaker.o lectVideoFrameCreate.o genericProcess.o computerProcess.o computerDistribute.o buffer.o ac.o
#gigE.o epiphanCapture.o 
LIB_OBJS= computerMain.o whiteBoard.o WebCamCapture.o usbCam.o paolMat.o module.o locateSpeaker.o lectVideoFrameCreate.o genericProcess.o computerProcess.o computerDistribute.o buffer.o ac.o
#gigE.o epiphanCapture.o

# Targets
all: libPaolThreads2.a PaolThreads2

# Make the bindir, libdir and infodir directories; 
#install: all
#	$(top_srcdir)/mkinstalldirs $(bindir)
#	$(top_srcdir)/mkinstalldirs $(libdir)
#	$(top_srcdir)/mkinstalldirs $(infodir)
#	$(INSTALL) PaolThreads2 $(bindir)/PaolThreads2
#	$(INSTALL) libPaolThreads2.a $(libdir)/libPaolThreads2.a
#	$(INSTALL) PaolThreads2.info $(infodir)/PaolThreads2.info

#uninstall:
#	/bin/rm -f $(bindir)/PaolThreads2
#	/bin/rm -f $(libdir)/libPaolThreads2.a
#        /bin/rm -f $(infodir)/PaolThreads2.info

libPaolThreads2.a: $(OBJS)
	/bin/rm -f libPaolThreads2.a
	$(AR) $(AR_FLAGS) libPaolThreads2.a $(LIB_OBJS)
	$(RANLIB) libPaolThreads2.a

PaolThreads2: $(OBJS) libPaolThreads2.a
	$(CC) $(CFLAGS) -o PaolThreads2 $(OBJS) $(LIBS)

clean:
	/bin/rm -f core *.o $(OBJS) $(LIB_OBJS) libPaolThreads2.a

distclean: clean
	/bin/rm -f Makefile config.h config.status config.cache config.log \
		marklib.dvi

mostlyclean: clean

maintainer-clean: clean

# automatic re-running of configure if the ocnfigure.in file has changed
${srcdir}/configure: configure.in 
	cd ${srcdir} && autoconf

# autoheader might not change config.h.in, so touch a stamp file
${srcdir}/config.h.in: stamp-h.in
${srcdir}/stamp-h.in: configure.in
		cd ${srcdir} && autoheader
		echo timestamp > ${srcdir}/stamp-h.in

config.h: stamp-h
stamp-h: config.h.in config.status
	./config.status
Makefile: Makefile.in config.status
	./config.status
config.status: configure
	./config.status --recheck
