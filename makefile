CXX = g++
DEBUG = -DDEBUG -g #-Wall
ifdef PROFILE
	PROF = -pg
endif
CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream ${PROF}
LDFLAGS = -L. -lz ${PROF}
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o wordinfoc.o dictionaryc.o HypothesisNode.o PartialTranslation.o aStar.o aStarElement.o
TESTS = wordinfoc.test.exe word.test.exe lexicon.test.exe dictionary.test.exe dictionaryc.test.exe

ifndef NOCOLORS
BLACK = \033[0m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
endif
default: all
all: singlewordextract.exe parse_singlewordextract.exe

%.test.exe: %.test.o ${OBJECTS}
	@${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o
	@echo -ne "${YELLOW}$@${BLACK}"
	@./$@ && echo -e " ${GREEN}ok${BLACK}"
	#@rm $@

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
