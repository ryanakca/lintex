# $Id: Makefile,v 1.3 2001/12/02 11:00:05 loreti Exp $

CFLAGS += -ansi -pedantic -Wall `pkg-config --cflags libconfig`
#CFLAGS = -ansi -Wall -g `pkg-config --cflags libconfig`
LIBS += `pkg-config --libs libconfig`

ROOT = /usr/local

.PHONY: install clean

lintex:	lintex.c Makefile
	$(CC) $(CXXFLAGS) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ lintex.c $(LIBS)

install: lintex
	strip lintex
	mv lintex   $(ROOT)/bin
	cp lintex.1 $(ROOT)/man/man1

lintex.pdf: lintex.1
	groff -man -Tps $< | ps2pdf - $@

clean:
	-rm *~ *.o core
	-rm lintex lintex.pdf
