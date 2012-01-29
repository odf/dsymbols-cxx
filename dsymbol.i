/* --------------------------------------------------------------------	*
 *	dsymbol.i			18-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


%module dsymbol

%{
#include "delaney_symbols.h"
#include <sstream>
#include <Python.h>

  typedef std::set<int>			int_set;
  typedef std::vector<int>		int_vec;
  typedef std::vector<int_vec>		int_vec2;
  typedef std::pair<int, int_vec>	edge;
  typedef PriorityQueue<edge, std::vector<edge>, std::greater<edge> > pqueue;

  typedef DelaneySymbol				GenericDSymbol;
  typedef SimpleDelaneySymbol			DSymbol;
  typedef DualDelaneySymbol			DualDSymbol;
  typedef SectionDelaneySymbol			SectionDSymbol;
  typedef DelaneySubsymbol			DSubsymbol;
  typedef TraversedDelaneySymbol<Queue<edge> >	DSymbolBFS;
  typedef TraversedDelaneySymbol<Stack<edge> >	DSymbolDFS;
  typedef TraversedDelaneySymbol<pqueue>	DSymbolEPS;
  typedef TraversedDelaneySymbol<IPBag<int> >	DSymbolIPS;
  typedef BinaryDelaneyCover	        	BinCoverDSymbol;
%}


%typemap(out) Answer {
  switch($1.theVal) {
  case Answer::Yes: $result = PyString_FromString("Yes"); break;
  case Answer::No:  $result = PyString_FromString("No"); break;
  default:          $result = PyString_FromString("Maybe");
  }
}

%typemap(in) int_set& {
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = new int_set;
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input, i);
      if (PyInt_Check(o))
	$1->insert((int)(PyInt_AsLong(o)));
      else {
	PyErr_SetString(PyExc_TypeError, "list must contain ints");
	return NULL;
      }
    }
  }
  else {
    PyErr_SetString(PyExc_TypeError, "not a list");
    return NULL;
  }
}

%typemap(in) int_vec& {
  if (PyList_Check($input)) {
    int size = PyList_Size($input);
    int i = 0;
    $1 = new int_vec;
    for (i = 0; i < size; i++) {
      PyObject *o = PyList_GetItem($input, i);
      if (PyInt_Check(o))
	$1->push_back((int)(PyInt_AsLong(o)));
      else {
	PyErr_SetString(PyExc_TypeError, "list must contain ints");
	return NULL;
      }
    }
  }
  else if (PyInt_Check($input)) {
    $1 = new int_vec;
    $1->push_back((int)(PyInt_AsLong($input)));
  }
  else {
    PyErr_SetString(PyExc_TypeError, "not a list");
    return NULL;
  }
}

%typemap(out) int_vec {
  if ($1.size() == 1)
    $result = PyInt_FromLong((long)($1[0]));
  else {
    int size = $1.size();
    int i;
    $result = PyList_New(size);
    for (i = 0; i < size; i++)
      PyList_SetItem($result, i, PyInt_FromLong((long)($1[i])));
  }
}

%typemap(out) int_vec2 {
  int n = $1.size();
  int i;
  $result = PyList_New(n);
  for (i = 0; i < n; i++) {
    int_vec x = $1[i];
    PyObject *y;
    if (x.size() == 1)
      y = PyInt_FromLong((long)(x[0]));
    else {
      int m = x.size();
      int j;
      y = PyList_New(m);
      for (j = 0; j < m; j++)
	PyList_SetItem(y, j, PyInt_FromLong((long)(x[j])));
    }
    PyList_SetItem($result, i, y);
  }
}


/*
  Each instance of this class contains a representation of Delaney
  symbol, where indices and elements are integers.
*/

class GenericDSymbol {
public:
  virtual ~GenericDSymbol() = 0;

  /* Returns the dimension of the symbol. */
  int dim() const;

  /* Returns the size of the symbol. */
  int size() const;

  /* Returns true if the symbol is finite (always), 0 otherwise. */
  bool is_finite () const;

  /* Returns true if i is a valid index, false otherwise. */
  bool idx_valid (const int i) const;
  /* Returns the first valid index. */
  int  idx_first () const;
  /* Returns an invalid index. */
  int  idx_none  () const;
  /* Returns the next valid index following i. */
  int  idx_next  (const int i) const;

  /* Returns true if D is a valid element, false otherwise. */
  bool elm_valid (const int_vec& D) const;
  /* Returns the first valid element. */
  int_vec  elm_first () const;
  /* Returns an invalid element. */
  int_vec  elm_none  () const;
  /* Returns the next valid element following D. */
  int_vec  elm_next  (const int_vec& D) const;

  /* Returns true if operation op(i) is defined on D, false otherwise. */
  bool op_defined (const int i, const int_vec& D) const;
  /* Returns the result of applying op(i) to D. */
  int_vec  op (const int i, const int_vec& D) const;

  /* Returns true if v(i,j) is defined on D, false otherwise. */
  bool v_defined (const int i, const int j, const int_vec& D) const;
  /* Returns the value of v(i,j) on D. */
  int  v (const int i, const int j, const int_vec& D) const;
  /* Returns true if the value of v(i,j) is forced on each element. */
  bool v_implied (const int i, const int j) const;

  /*
    Returns the smallest positive number r such that application of the
    r-th power of the product op(i)op(j) on D yields D.
  */
  int  r(const int i, const int j, const int_vec& D) const;
  /* Returns v(i,j,D)*r(i,j,D). */
  int  m(const int i, const int j, const int_vec& D) const;

  /*
    Returns true if op and v are defined for all legal index-element
    combinations, false otherwise.
  */
  Answer is_complete        () const;

  /*
    Returns true if for each pair of different non-consecutive indices
    i and j and each element D we have op(i)op(j) = op(j)op(i) and, if
    applicable, m(i,j,D) = 2, false otherwise.
  */
  Answer is_proper          () const;

  /*
    Returns true if op(i, D) != D for all indices i and elements D,
    false otherwise.
  */
  Answer is_loopless        () const;

  /*
    Returns true if v is defined and equal to 1 for all index-element
    combinations, false otherwise.
  */
  Answer is_simple          () const;

  /*
    Returns true if there is a path, i.e. a connection by a sequence
    of op-applications, between each pair of elements, false
    otherwise.
  */
  Answer is_connected       () const;

  /*
    Returns true if there is a weak orientation on the set of
    elements, false otherwise. A weak orientation is a boolean-valued
    function d such that op(i,D) != D implies d(op(i,D)) != d(D) for
    all indices i and all elements D.
  */
  Answer is_weakly_oriented () const;

  /*
    Returns true if there is an orientation on the set of elements,
    false otherwise. An orientation is a boolean-valued function d
    such that d(op(i,D)) != d(D) holds for all indices i and all
    elements D. A symbol is oriented exactly if it is both weakly
    oriented and loopless.
  */
  Answer is_oriented        () const;

  /*
    Produces a textual representation of the symbol to be used by
    Python's print function.
  */
  %extend {
    const char* __str__() {
      std::ostringstream buf;
      self->print(buf);

      std::string str = buf.str();
      char* cstr = new char [str.size()+1];
      strcpy (cstr, str.c_str());
      return cstr;
    }
  }
};


/*
  This derived class adds methods to modify a given Delaney symbol.
*/

class DSymbol : public GenericDSymbol {
public:
  /* Constructs a dynamic Delaney symbol of dimension dim and size sz. */
  DSymbol(int dim, int sz);

  /*
    Makes op(i) undefined on D. To keep the symbol consistent, op(i)
    is also made undefined on the former image of op(i) on D. Returns
    true on success, false otherwise.
  */
  bool  unset_op(const int i, const int_vec& D);

  /*
    Sets op(i,D) to Di and op(i,Di) to D. Returns true on success,
    false otherwise.
  */
  bool  set_op(const int i, const int_vec& D, const int_vec& Di);

  /*
    Makes v(i,j) undefined on the i,j-orbit containing D. Returns true
    on success, false otherwise.
  */
  bool  unset_v(const int i, const int j, const int_vec& D);

  /*
    Sets v(i,j) to v on the i,j-orbit containing D. Returns true on
    success, false otherwise.
  */
  bool  set_v(const int i, const int j,	const int D_vec, const int v);

  /*
    Adds D to the set of elements. Returns true on success, false
    otherwise.
  */
  bool  add_element(const int_vec& D);

  /*
    Removes D from the set of elements. Returns true on success, false
    otherwise.
  */
  bool  remove_element(const int_vec& D);

  /* Returns an identical copy of the given symbol. */
  %extend {
    DSymbol clone() {
      return DSymbol(*self);
    }
  }
};


/*
  The dual of a Delaney symbol is obtained by reversing the indices.
*/
class DualDSymbol : public GenericDSymbol {
public:
  /* Constructs a dual symbol from any given symbol. */
  DualDSymbol(const GenericDSymbol& ds);
};


/*
  A section is obtained by keeping only a subset of the operations.
*/
class SectionDSymbol : public GenericDSymbol {
public:
  /* Constructs a section from a base symbol and a list of indices. */
  SectionDSymbol(const GenericDSymbol& ds, const int_set& indices);
};


/*
  A class for sub-symbols.
*/
class DSubsymbol : public GenericDSymbol {
public:
  /*
    Constructs a sub-symbol from a base symbol, a list of indices and
    a seed element.
  */
  DSubsymbol(const GenericDSymbol& ds,
	     const int_set& indices,
	     const int_vec& seed);
};


/*
  In a DSymbolBFS, the order of the elements is determined by a
  'breadth first' tree search of the Delaney graph.
*/
class DSymbolBFS : public GenericDSymbol {
public:
  /*
    Constructs a DSymbolBFS from an ordinary one. If no seed is given,
    the first element of the base symbol is used.
  */
  DSymbolBFS(const GenericDSymbol& ds, const int_vec& seed);

  /*
    Returns the index of an operator which swaps D with its tree
    predecessor.
  */
  int idx_back_edge(const int_vec& D) const;
};

/*
  In a DSymbolDFS, the order of the elements is determined by a 'depth
  first' tree search of the Delaney graph.
*/
class DSymbolDFS : public GenericDSymbol {
public:
  /*
    Constructs a DSymbolDFS from an ordinary one. If no seed is given,
    the first element of the base symbol is used.
  */
  DSymbolDFS(const GenericDSymbol& ds, const int_vec& seed);

  /*
    Returns the index of an operator which swaps D with its tree
    predecessor.
  */
  int idx_back_edge(const int_vec& D) const;
};


/*
  In a DSymbolEPS, the order of the elements is determined by an
  'edge priority' tree search of the Delaney graph.
*/
class DSymbolEPS : public GenericDSymbol {
public:
  /*
    Constructs a DSymbolEPS from an ordinary one. If no seed is given,
    the first element of the base symbol is used.
  */
  DSymbolEPS(const GenericDSymbol& ds, const int_vec& seed);

  /*
    Returns the index of an operator which swaps D with its tree
    predecessor.
  */
  int idx_back_edge(const int_vec& D) const;
};


/*
  In a DSymbolIPS, the order of the elements is determined by an
  'index priority' tree search of the Delaney graph. This ordering is
  nice for humans to read, especially when converted back to a simple
  symbol.
*/
class DSymbolIPS : public GenericDSymbol {
public:
  /*
    Constructs a DSymbolIPS from an ordinary one. If no seed is given,
    the first element of the base symbol is used.
  */
  DSymbolIPS(const GenericDSymbol& ds, const int_vec& seed);

  /*
    Returns the index of an operator which swaps D with its tree
    predecessor.
  */
  int idx_back_edge(const int_vec& D) const;
};


/*
  The binary cover of a Delaney symbol has two layers with values 0
  and 1. Each operation swaps layers.
*/
class BinCoverDSymbol : public GenericDSymbol {
public:
  /*
    Construct the binary cover of a given symbol.
  */
  BinCoverDSymbol(const GenericDSymbol& ds);
};


/*
  This function constructs to a given Delaney symbol a new one with
  the same abstract structure but consecutive scalars as elements.
*/
DSymbol
simple_ds(const GenericDSymbol& src);

/*
  This function returns a list of representative elements for the
  connected components of the given Delaney symbol.
*/
int_vec2
RepresentativesComponents(const GenericDSymbol& ds);

/*
** Local Variables:
** mode:C++
** compile-command: "make dsymbol.py"
** End:
*/

/* --- EOF dsymbol.i --- */
