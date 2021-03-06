CXX = g++
DEBUG = -DDEBUG -g -Wall
ifdef PROFILE
	PROF = -pg
endif
ifndef NPARALLEL
	OMP=-fopenmp
endif

ifdef EECHO
	E=-e
endif

CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream ${PROF} ${OMP} ${OPTIMIZE}
LDFLAGS = -L. -lz ${PROF} ${OMP} ${OPTIMIZE}
OBJECTS = word.o lexicon.o wordinfo.o dictionary.o wordinfoc.o dictionaryc.o HypothesisNode.o PartialTranslation.o aStar.o aStarElement.o levenshtein.o sentencepool.o ptree.o cost.o decoder.o

TESTS = wordinfoc.test.exe word.test.exe lexicon.test.exe dictionary.test.exe dictionaryc.test.exe levenshtein.test.exe sentencepool.test.exe PER_WER.test.exe bleu.test.exe ptree.test.exe aStar.test.exe

ifndef NOCOLORS
BLACK = \033[0m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
endif
default: all
all: singlewordextract.exe bewertung.exe translate.exe phraseextract.exe optimize.exe

%.test.exe: %.test.o ${OBJECTS}
	@${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o
	@echo $E -n "${YELLOW}$@${BLACK}"
	@./$@ && echo $E " ${GREEN}ok${BLACK}"

%.exe: %.o ${OBJECTS}
	${CXX} ${LDFLAGS} -o $@ $< ${OBJECTS} gzstream/gzstream.o

.PHONY: tests
tests: ${TESTS}
	@echo $E "\n${GREEN}--> everything's good! <--${BLACK}"

.PRECIOUS: %.test.o
%.test.o: %.test.cpp %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<

.PRECIOUS: %.o
%.o: %.cpp %.h
	${CXX} ${CPPFLAGS} -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o *.exe

.PHONY: ctests
ctests:
	rm -f ${TESTS}
	${MAKE} tests
