# makefile - Build system for fractal render
#
# @author: Cade Brown <cade@cade.site>


# -*- Config -*-

CXXFLAGS += -Ofast

# -*- Input -*-

src_CC     := $(wildcard src/*.cc)
src_HH     := $(wildcard src/*.hh)

# -*- Output -*-

src_O      := $(patsubst %.cc,%.o,$(src_CC))

EXE        := fractalrender


$(EXE): $(src_O)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cc $(src_HH)
	$(CXX) $(CXXFLAGS) -Isrc -c -fPIC -o $@ $<


