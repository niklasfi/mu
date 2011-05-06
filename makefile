CXX = g++
DEBUG = -DDEBUG -g -Wall
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o wordinfoc.o
TESTS = wordinfoc.test.exe word.test.exe lexicon.test.exe

all: singlewordextract.exe

%.test.exe: %.test.o ${OBJECTS}
	${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o
	@echo -ne "\033[33m$@\033[0m"
	@./$@ && echo -e " \033[32mok\033[0m"
	@rm $@

%.exe: %.o ${OBJECTS}
	${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o

tests: ${TESTS}

.PRECIOUS: %.test.o
%.test.o: %.test %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<
	
.PRECIOUS: %.o
%.o: %.cpp %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<
	
clean:
	rm -f *.o *.exe
