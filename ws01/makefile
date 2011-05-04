CXX = g++
DEBUG = -DDEBUG -g -Wall
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o

default: ${OBJECTS} main.o
	${CXX} ${LDFLAGS} -o run.exe main.o ${OBJECTS} gzstream/gzstream.o

all: ${OBJECTS} main.o
	${CXX} ${LDFLAGS} -o singleWordExtract main.o ${OBJECTS} gzstream/gzstream.o

tests: ${OBJECTS} tests.o
	${CXX} ${LDFLAGS} -o tests.exe tests.o ${OBJECTS} gzstream/gzstream.o
	#run the tests
	./tests.exe
	#ok!

a02: ${OBJECTS} a02.o
	${CXX} ${LDFLAGS} -o a02.exe a02.o ${OBJECTS} gzstream/gzstream.o

igzexample: ${OBJECTS} igzexample.o
	${CXX} ${LDFLAGS} -o igzexample.exe igzexample.o ${OBJECTS} gzstream/gzstream.o

main.o: main.cpp main.h lexicon.h language.h word.h wordinfo.h dictionary.h
	${CXX} ${CPPFLAGS} main.cpp

a02.o: a02.cpp main.h lexicon.h language.h word.h wordinfo.h dictionary.h
	${CXX} ${CPPFLAGS} a02.cpp

tests.o: tests.cpp tests.h lexicon.h language.h word.h
	${CXX} ${CPPFLAGS} tests.cpp

igzexample.o: igzexample.cpp
	${CXX} ${CPPFLAGS} igzexample.cpp

word.o: word.cpp word.h language.h
	${CXX} ${CPPFLAGS} word.cpp

lexicon.o: lexicon.cpp lexicon.h word.h language.h
	${CXX} ${CPPFLAGS} lexicon.cpp
	
wordinfo.o: wordinfo.cpp wordinfo.h word.h
	${CXX} ${CPPFLAGS} wordinfo.cpp
	
dictionary.o: dictionary.cpp dictionary.h lexicon.h wordinfo.h word.h language.h
	${CXX} ${CPPFLAGS} dictionary.cpp

clean:
	rm -f *.o *.exe
