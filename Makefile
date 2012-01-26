# --------------------

INCPATH   = -I. -I$(HOME)/include -I$(HOME)/include/c++
LIBPATH   = -L. -L$(HOME)/lib
FLAGS     = -g
CXXFLAGS  = $(FLAGS) $(INCPATH)

LIBS      = -lgmp -lm


# --------------------

.SUFFIXES:	.o .cc .i .py

.o:
	$(CXX) $(CXXFLAGS) -o $* $*.o $(LIBPATH) $(LIBS)

.cc.o:
	$(CXX) $(CXXFLAGS) -c $*.cc

.cc:
	$(CXX) $(CXXFLAGS) -o $* $*.cc $(LIBPATH) $(LIBS)

.i.py:
	swig -python -c++ -shadow $*.i
	$(CXX) $(CXXFLAGS) -I/usr/include/python2.7 $*_wrap.cxx \
		-shared -o _$*.so
	rm $*_wrap.cxx

# --------------------

main:	testds

depend:
	makedepend -Y. testds.cc

# --------------------
# DO NOT DELETE

testds.o: Answer.h delaney_symbols.h Bag.h IPBag.h Integer.h Rational.h
testds.o: DelaneySymbol.h DynamicDelaneySymbol.h SimpleDelaneySymbol.h
testds.o: IndirectDelaneySymbol.h DualDelaneySymbol.h SectionDelaneySymbol.h
testds.o: DelaneySubsymbol.h TraversedDelaneySymbol.h DelaneyCover.h
testds.o: BinaryDelaneyCover.h
