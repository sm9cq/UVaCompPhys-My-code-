# This Makefile builds two stand alone programs that have access to 
# the ROOT libraries.  To use this with your own code just substitute
# the name of your program below.

# here we access the root configuration, include files, and libraries
ROOTCFLAGS=$(shell root-config --cflags)
ROOTINC=$(shell root-config --incdir)
ROOTLIBDIR=$(shell root-config --libdir)
ROOTLIBS=$(shell root-config --libs) -lMinuit
ROOTLDFLAGS=$(shell root-config --ldflags)

ROOTC=$(ROOTCFLAGS) 
#-I$(ROOTINC)
ROOTLINK=-L$(ROOTLIBDIR) $(ROOTLIBS) $(ROOTLDFLAGS)

CPP=g++

default: expFit rootExample minuit1 expFit2 test minuit1_2 2dfit test2d

expFit: expFit.cpp
	$(CPP) -O -Wall $(ROOTC) -o expFit expFit.cpp $(ROOTLINK) 

test: test.cpp
	$(CPP) -O -Wall $(ROOTC) -o test test.cpp $(ROOTLINK)
test2d: test2d.cpp
	$(CPP) -O -Wall $(ROOTC) -o test2d test2d.cpp $(ROOTLINK)

rootExample: rootExample.cpp
	$(CPP) -O -Wall $(ROOTC) -o rootExample rootExample.cpp $(ROOTLINK) 
# note: just replace the -O flag with -g to build a debug version
minuit1: minuit1.cpp
	$(CPP) -O -Wall $(ROOTC) -o minuit1 minuit1.cpp $(ROOTLINK) 
minuit1_2: minuit1_2.cpp
	$(CPP) -O -Wall $(ROOTC) -o minuit1_2 minuit1_2.cpp $(ROOTLINK)

2dfit: 2dfit.cpp
	$(CPP) -O -Wall $(ROOTC) -o 2dfit 2dfit.cpp $(ROOTLINK)

clean: 
	rm -f expFit rootExample minuit1 minuit1_2 2dfit test test2d *~ *.d *.so
