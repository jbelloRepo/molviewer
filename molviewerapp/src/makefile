CC = gcc
CFLAGS = -Wall -pedantic -std=c99
# DEPS = mol.h
LIBS=-lm # note: the l means library, m means math

# all: main
all: myprog 

clean:  
	rm -f *.o *.so myprog 

# Object file for mol.c	
mylib.o:  mol.c mol.h
	$(CC) $(CFLAGS) -c mol.c -fPIC -o mylib.o

# Shared library 
libmol.so: mylib.o
	$(CC) mylib.o -shared -o libmol.so $(LIBS)

# 1. Generates python interface with C-code
molecule_python: 
	swig -python molecule.i

# 2. Compiles *_wrap.c file 
molecule_wrap.o: molecule_wrap.c
	$(CC) $(FLAGS) -c -fPIC -I/usr/include/python3.10 molecule_wrap.c -o molecule_wrap.o

# 3. 
_molecule.so: libmol.so molecule_wrap.o
	$(CC) $(FLAGS) -dynamiclib -shared molecule_wrap.o -o _molecule.so -L. -lmol -L/usr/lib/python3.7/config-3.7m-x86_64-linux-gnu 

libpath:
	export LD_LIBRARY_PATH=`pwd`

# For testing 
main.o:  main.c mol.h
	$(CC) $(FLAGS) -c main.c -o main.o 

myprog: molecule_python main.o _molecule.so libmol.so libpath
	$(CC) main.o -L. -l mol -o myprog $(LIBS)


# export LD_LIBRARY_PATH=`pwd`
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd):/usr/lib/python3.7/config-3.7m-x86_64-linux-gnu/:/usr/include/python3.7m



# docker run -p 57766:57766 --rm -it cis2750
# sudo docker build . -t cis2750  
# python3 sever.py 57766