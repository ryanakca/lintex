# Author: Maurizio Loreti, aka MLO or (HAM) I3NOO
# Work:   University of Padova - Department of Physics
#         Via F. Marzolo, 8 - 35131 PADOVA - Italy
# Phone:  +39 (049) 827-7216   FAX: +39 (049) 827-7102
# EMail:  loreti@pd.infn.it
# WWW:    http://www.pd.infn.it/~loreti/mlo.html
#
# $Id: Makefile,v 1.9 2005/01/21 08:16:48 loreti Exp $
#
######################################################

.PHONY: clean

CXX = g++
#CXXFLAGS = -std=c++98 -pedantic -W -Wall -g -DDEBUG
CXXFLAGS = -std=c++98 -pedantic -W -Wall -O2

#CXX = KCC
#CXXFLAGS = -O -DDEBUG
#CXXFLAGS = -O

LDFLAGS =

ltx: ltx.o cleandir.o cleanup.o file.o
	$(CXX) $(LDFLAGS) -o $@ ltx.o cleandir.o cleanup.o file.o

ltx.o: ltx.cxx ltx.hh cleandir.hh
	$(CXX) $(CXXFLAGS) -o $@ -c ltx.cxx

cleandir.o: cleandir.cxx cleandir.hh cleanup.hh
	$(CXX) $(CXXFLAGS) -o $@ -c cleandir.cxx

cleanup.o: cleanup.cxx cleanup.hh file.hh
	$(CXX) $(CXXFLAGS) -o $@ -c cleanup.cxx

file.o: file.cxx file.hh
	$(CXX) $(CXXFLAGS) -o $@ -c file.cxx

clean:
	-rm *~ *.o ltx
	-if [ -d ti_files ]; then rm ti_files/* && rmdir ti_files; fi
