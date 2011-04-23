CXX = g++
DEBUG = -DDEBUG
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = main.o word.o lexicon.o

default: ${OBJECTS}
	${CXX} ${LDFLAGS} -o run.exe ${OBJECTS} gzstream/gzstream.o

main.o: main.cpp main.h lexicon.h
	${CXX} ${CPPFLAGS} main.cpp

word.o: word.cpp word.h
	${CXX} ${CPPFLAGS} word.cpp

lexicon.o: lexicon.cpp lexicon.h word.h
	${CXX} ${CPPFLAGS} lexicon.cpp

clean:
	rm -f *.o run.exe
