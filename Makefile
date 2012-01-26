# --------------------

CXXFLAGS  = -O -g
PYHEADERS = /usr/include/python2.7

# --------------------

.SUFFIXES:	.o .cxx .i .py

.cxx.o:
	$(CXX) $(CXXFLAGS) -c $*.cxx

.i.py:
	swig -python -c++ -shadow $*.i
	$(CXX) $(CXXFLAGS) -I$(PYHEADERS) $*_wrap.cxx -shared -o _$*.so
	rm $*_wrap.cxx

# --------------------

testds:	testds.o
	$(CXX) $(CXXFLAGS) -o testds testds.o -lgmp -lm

depend:
	makedepend -Y. testds.cxx

clean:
	rm -f *.o *.so *.pyc dsymbol.py dsymbol_wrap.cxx

# --------------------
# DO NOT DELETE

testds.o: Answer.h delaney_symbols.h Bag.h IPBag.h Integer.h Rational.h
testds.o: DelaneySymbol.h DynamicDelaneySymbol.h SimpleDelaneySymbol.h
testds.o: IndirectDelaneySymbol.h DualDelaneySymbol.h SectionDelaneySymbol.h
testds.o: DelaneySubsymbol.h TraversedDelaneySymbol.h DelaneyCover.h
testds.o: BinaryDelaneyCover.h
