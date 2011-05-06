CXX = g++
DEBUG = -DDEBUG -g -Wall
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o wordinfoc.o
TESTS = wordinfoc.test.exe word.test.exe
all: singlewordextract.exe

%.exe: %.o ${OBJECTS}
	${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o

tests: ${TESTS}
	@for i in *.test.exe; do (echo -n "\033[33m$$i\033[0m " && ./$$i && echo "\033[32mok\033[0m") || (echo "\033[31merror\033[0m" && exit 2); done;
	@echo "\033[32meverything is ok!\033[0m"

.PRECIOUS: %.test.o
%.test.o: %.test %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<
	
.PRECIOUS: %.o
%.o: %.cpp %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<
	
clean:
	rm -f *.o *.exe
