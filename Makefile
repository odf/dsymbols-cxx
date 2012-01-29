# --------------------

CXXFLAGS  = -O3 -g
PYHEADERS = /usr/include/python2.7

# --------------------

.SUFFIXES:	.i .py

.i.py:
	swig -python -c++ -shadow $*.i
	$(CXX) $(CXXFLAGS) -I$(PYHEADERS) $*_wrap.cxx -shared -lgmp -lm -o _$*.so
	rm $*_wrap.cxx

# --------------------

testds:	testds.o
	$(CXX) $(CXXFLAGS) -o testds testds.o -lgmp -lm

test:	testds
	./testds | diff - test.out && echo "OK!"

depend:
	makedepend -Y. testds.cpp dsymbol.i

clean:
	rm -f *.o *.so *.pyc dsymbol.py dsymbol_wrap.cxx

distclean: clean
	rm -f testds

# --------------------
# DO NOT DELETE

testds.o: Answer.h delaney_symbols.h Bag.h IPBag.h Integer.h shared_array.hpp
testds.o: Rational.h DelaneySymbol.h DynamicDelaneySymbol.h
testds.o: SimpleDelaneySymbol.h IndirectDelaneySymbol.h DualDelaneySymbol.h
testds.o: SectionDelaneySymbol.h DelaneySubsymbol.h TraversedDelaneySymbol.h
testds.o: DelaneyCover.h BinaryDelaneyCover.h
dsymbol.py: delaney_symbols.h Answer.h Bag.h IPBag.h Integer.h
dsymbol.py: shared_array.hpp Rational.h DelaneySymbol.h DynamicDelaneySymbol.h
dsymbol.py: SimpleDelaneySymbol.h IndirectDelaneySymbol.h DualDelaneySymbol.h
dsymbol.py: SectionDelaneySymbol.h DelaneySubsymbol.h TraversedDelaneySymbol.h
dsymbol.py: DelaneyCover.h BinaryDelaneyCover.h
