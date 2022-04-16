# here we access the root configuration, include files, and libraries
ROOTCFLAGS=$(shell root-config --cflags)
ROOTINC=$(shell root-config --incdir)
ROOTLIBDIR=$(shell root-config --libdir)
ROOTLIBS=$(shell root-config --libs) -lMinuit -lMathMore
ROOTLDFLAGS=$(shell root-config --ldflags)

ROOTC=$(ROOTCFLAGS) 
#-I$(ROOTINC)
ROOTLINK=-L$(ROOTLIBDIR) $(ROOTLIBS) $(ROOTLDFLAGS)


a11: Sphere-Cube5d


Integral5d: Integral5d.cpp 
#	g++ -g $(ROOTC) -o Integral5d Integral5.cpp $(ROOTLINK)
	g++ -O3 -Wall $(ROOTC) -o Integral5d Integral5d.cpp $(ROOTLINK)
Sphere-Cube5d: Sphere-Cube5d.cpp

	g++ -O3 -Wall $(ROOTC) -o Sphere-Cube5d Sphere-Cube5d.cpp $(ROOTLINK)

clean:
	rm -f Integral5d Sphere-Cube5d
	rm -f *~ *.d *.so *.pcm rm *.log

cleanall: clean
	rm -r *root *png *pdf
