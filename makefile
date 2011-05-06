CXX = g++
DEBUG = -DDEBUG -g -Wall
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o wordinfoc.o
TESTS = wordinfoc.test.exe
all: singlewordextract.exe

%.exe: %.o ${OBJECTS}
	${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o

tests: ${TESTS}
	#it's ok, if it ends with #ok
	for i in *.test.exe; do	./$$i || (echo "something is wrong with $$i" && exit 1); done;
	#ok!

.PRECIOUS: %.test.o
%.test.o: %.test %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<
	
.PRECIOUS: %.o
%.o: %.cpp %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<
	
clean:
	rm -f *.o *.exe
