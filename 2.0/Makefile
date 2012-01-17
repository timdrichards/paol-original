SHELL = /bin/sh
top_srcdir = .
srcdir = .


.SUFFIXES:
.SUFFIXES: .cpp .o

OPT=-g -O

AR = ar
AR_FLAGS = rc
RANLIB = ranlib

CC = gcc
CFLAGS = -I. -lboost-thread -g -O2
LDFLAGS = 
LIBS = 
#INSTALL = /usr/bin/install -c
prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
infodir = $(prefix)/info

# Sources
SOURCES=main.cpp whiteBoard.cpp WebCamCapture.cpp usbCam.cpp paolMat.cpp module.cpp locateSpeaker.cpp lectVideoFrameCreate.cpp genericProcess.cpp computerProcess.cpp computerDistribute.cpp buffer.cpp ac.cpp
#gigE.cpp epiphanCapture.cpp
DOCS=PaolThreads2.texi PaolThreads2.info
MISC=configure mkinstalldirs install-sh
OBJS=main.o whiteBoard.o WebCamCapture.o usbCam.o paolMat.o module.o locateSpeaker.o lectVideoFrameCreate.o genericProcess.o computerProcess.o computerDistribute.o buffer.o ac.o
#gigE.o epiphanCapture.o 
LIB_OBJS=whiteBoard.o WebCamCapture.o usbCam.o paolMat.o module.o locateSpeaker.o lectVideoFrameCreate.o genericProcess.o computerProcess.o computerDistribute.o buffer.o ac.o
#gigE.o epiphanCapture.o

# Targets
all: libPaolThreads2.a PaolThreads2 PaolThreads2.info

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

PaolThreads2.info: PaolThreads2.texi
	makeinfo PaolThreads2.texi

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
