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

include makefile.local
#hier SRILM_PATH (pfad zur srilm bib)
#und MACHINE_TYPE (usually output of uname -m) setzen

CPPFLAGS = ${DEBUG} -std=c++0x -c -I. -Igzstream -I${SRILM_PATH}/include ${PROF} ${OMP} ${OPTIMIZE}
LDFLAGS = -L. -lz -loolm -ldstruct -lflm -lmisc -L${SRILM_PATH}/lib/${MACHINE_TYPE} ${PROF} ${OMP} ${OPTIMIZE}
OBJECTS = HypothesisNode.o PartialTranslation.o aStar.o aStarElement.o ptree.o cost.o decoder.o

TESTS = levenshtein.test.exe sentencepool.test.exe PER_WER.test.exe bleu.test.exe ptree.test.exe aStar.test.exe

ifndef NOCOLORS
BLACK = \033[0m
RED = \033[31m
GREEN = \033[32m
YELLOW = \033[33m
endif
default: all
all:  translate.exe 

%.test.exe: %.test.o ${OBJECTS}
	@${CXX}  -o $@ $< ${OBJECTS} gzstream/gzstream.o ${LDFLAGS}
	@echo $E -n "${YELLOW}$@${BLACK}"
	@./$@ && echo $E " ${GREEN}ok${BLACK}"

%.exe: %.o ${OBJECTS}
	${CXX}  -o $@ $< ${OBJECTS} gzstream/gzstream.o ${LDFLAGS}

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
