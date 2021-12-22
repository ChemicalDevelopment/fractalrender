# makefile - Build system for fractal render
#
# @author: Cade Brown <cade@cade.site>


# -*- Config -*-

#CXXFLAGS += -Ofast -g
CXXFLAGS += -Ofast


# -*- Input -*-

src_CC     := $(wildcard src/*.cc)
src_HH     := $(wildcard include/*.hh)

# -*- Output -*-

src_O      := $(patsubst %.cc,%.o,$(src_CC))

EXE        := fractalrender


.PHONY: all clean FORCE

all: $(EXE)

clean: FORCE
	rm -f $(wildcard $(EXE) $(src_O))

FORCE:

$(EXE): $(src_O)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc $(src_HH)
	$(CXX) $(CXXFLAGS) -Iinclude -c -fPIC -o $@ $<


