CXX = g++
DEBUG = -DDEBUG -g #-Wall
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream
LDFLAGS = -L. -lz
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o wordinfoc.o dictionaryc.o
TESTS = wordinfoc.test.exe word.test.exe lexicon.test.exe dictionary.test.exe dictionaryc.test.exe

ifndef NOCOLORS
BLACK = \033[0m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
endif

all: singlewordextract.exe parse_singlewordextract.exe

%.test.exe: %.test.o ${OBJECTS}
	@${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o
	@echo -ne "${YELLOW}$@${BLACK}"
	@./$@ && echo -e " ${GREEN}ok${BLACK}"
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
