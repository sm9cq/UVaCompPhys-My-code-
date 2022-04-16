# very basic example of a Makefile
# xxx: These are targets, the first one is the default
#      Typing "make target" will execute specific build instructions 
#      Following the target are dependencies.  If the files designated
#      as dependencies have not changed since running the previous make,
#      then the code will not be rebuilt.  This is very useful if many
#      files need to be compiled for a large program.  Only updates and
#      files depending on them will be recompiled.
#    
#      Following the target line are lines beginning w/ a TAB character.
#      These lines define the build (or other instructions)

all: mexp mexp.so mexp.soc

# starter for exp(-x) caculation
mexp: mexp.cpp mexp.h
	g++ -O -Wall -omexp mexp.cpp    # build executable

# Below we build shared libraries that can be laoded and used in Python

# We build two versions of the shared library
# Nothing special is required to load the C++ libraries into python using ROOT
# If we use ctypes and load the functions by hand in python, they must be
# linked using the extern "C" setting.  Otherwise python will not be able to
# recognize the names of the functions (related to C++ name mangling)

mexp.so: mexp.cpp mexp.h
	g++ -O -Wall -fPIC -shared -omexp.so mexp.cpp # build shared library
mexp.soc: mexp.cpp mexp.h  # build "so" w/ C style linkage
	g++ -O -Wall -fPIC -DEXTERNC -shared -omexp.soc mexp.cpp

test: mexp.soc
	python .github/test.py -t0 > .github/test0.tmp
	python .github/test.py -t1 > .github/test1.tmp	
	python .github/test.py -t2 > .github/test2.tmp	
	python .github/test.py -t3 > .github/test3.tmp	
	python .github/test.py -t4 > .github/test4.tmp	
	python .github/test.py -t5 > .github/test5.tmp
	python .github/test.py -t6 > .github/test6.tmp
	cat .github/test*tmp


clean:
	rm -f mexp mexp.so mexp.soc *png *pdf .github/test*.tmp *~

