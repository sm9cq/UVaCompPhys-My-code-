# We build two versions of the shared library
# Nothing special is required to load the C++ libraries into python using ROOT
# If we use ctypes and load the functions by hand in python, they must be
# linked using the extern "C" setting.  Otherwise python will not be able to
# recognize the names of the functions (c.f. name mangling)


all: mexp mexpso mexpsoc

mexp: mexp.cpp mexp.h
	g++ -O -Wall -omexp mexp.cpp    # build executable
mexpso: mexp.cpp mexp.h
	g++ -O -Wall -fPIC -shared -omexp.so mexp.cpp # build shared library
mexpsoc: mexp.cpp mexp.h  # build "so" w/ C style linkage (no name mangling)
	g++ -O -Wall -fPIC -DEXTERNC -shared -omexp.soc mexp.cpp
# use nm -D mexp.so(c) to see exported functions

clean:
	rm -rf mexp *.soc *.so *.root *.pcm *.d *.pyc __pycache__ *.pdf *#
