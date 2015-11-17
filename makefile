IDIR=include
CXX=g++
DEBUG=-g
CFLAGS=-std=c++0x -c -o
LFLAGS=-std=c++0x $(DEBUG)
SDIR=src
ODIR=obj
LDIR=lib
LIBS=-lm
BDIR=bin
DEPS=$(ODIR)/dictionary.o $(ODIR)/kb.o $(ODIR)/trie.o $(ODIR)/neuron.o $(ODIR)/neuronpool.o $(ODIR)/topology.o 

all: $(BDIR)/training $(BDIR)/testing

$(ODIR)/kb.o:$(SDIR)/kb.cpp $(IDIR)/kb.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/dictionary.o:$(SDIR)/dictionary.cpp $(IDIR)/dictionary.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/neuron.o:$(SDIR)/neuron.cpp $(IDIR)/neuron.h $(IDIR)/dictionary.h $(IDIR)/kb.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/trie.o:$(SDIR)/trie.cpp $(IDIR)/trie.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/neuronpool.o:$(SDIR)/neuronpool.cpp $(IDIR)/neuronpool.h $(IDIR)/neuron.h $(IDIR)/dictionary.h $(IDIR)/kb.h $(IDIR)/trie.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/topology.o:$(SDIR)/topology.cpp $(IDIR)/topology.h $(IDIR)/dictionary.h $(IDIR)/trie.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/training.o:$(SDIR)/training.cpp $(IDIR)/training.h $(IDIR)/neuronpool.h $(IDIR)/dictionary.h $(IDIR)/kb.h $(IDIR)/trie.h $(IDIR)/topology.h $(IDIR)/parameter.h
	$(CXX) $(CFLAGS) $@ $<
$(ODIR)/testing.o:$(SDIR)/testing.cpp $(IDIR)/testing.h $(IDIR)/neuronpool.h $(IDIR)/dictionary.h $(IDIR)/kb.h $(IDIR)/trie.h $(IDIR)/topology.h $(IDIR)/parameter.h
	$(CXX) $(CFLAGS) $@ $<

$(BDIR)/training:$(ODIR)/training.o $(DEPS)
	$(CXX) -o $@ $^ $(LFLAGS)
$(BDIR)/testing:$(ODIR)/testing.o $(DEPS)
	$(CXX) -o $@ $^ $(LFLAGS)
 
.PHONY: clean

clean:
	rm -f $(BDIR)/* $(ODIR)/*.o 


