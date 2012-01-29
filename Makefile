# --------------------

CXXFLAGS  = -O -g
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
	makedepend -Y. testds.cxx

clean:
	rm -f *.o *.so *.pyc dsymbol.py dsymbol_wrap.cxx

distclean: clean
	rm -f testds

# --------------------
# DO NOT DELETE

testds.o: Answer.h delaney_symbols.h Bag.h IPBag.h Integer.h Rational.h
testds.o: DelaneySymbol.h DynamicDelaneySymbol.h SimpleDelaneySymbol.h
testds.o: IndirectDelaneySymbol.h DualDelaneySymbol.h SectionDelaneySymbol.h
testds.o: DelaneySubsymbol.h TraversedDelaneySymbol.h DelaneyCover.h
testds.o: BinaryDelaneyCover.h
