CXX = g++
DEBUG = -DDEBUG -g -Wall
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = word.o lexicon.o

default: ${OBJECTS} main.o
	${CXX} ${LDFLAGS} -o run.exe main.o ${OBJECTS} gzstream/gzstream.o

tests: ${OBJECTS} tests.o
	${CXX} ${LDFLAGS} -o tests.exe tests.o ${OBJECTS} gzstream/gzstream.o
	#run the tests
	./tests.exe
	#ok!

main.o: main.cpp main.h lexicon.h language.h word.h
	${CXX} ${CPPFLAGS} main.cpp

tests.o: tests.cpp tests.h lexicon.h language.h word.h
	${CXX} ${CPPFLAGS} tests.cpp

word.o: word.cpp word.h language.h
	${CXX} ${CPPFLAGS} word.cpp

lexicon.o: lexicon.cpp lexicon.h word.h language.h
	${CXX} ${CPPFLAGS} lexicon.cpp

clean:
	rm -f *.o *.exe
