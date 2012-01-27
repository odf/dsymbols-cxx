/* --------------------------------------------------------------------	*
 *	DelaneySymbol.h			07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_DelaneySymbol
#define _h_DelaneySymbol 1

#include <stddef.h>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include "Answer.h"


// ------------------------------------------------------------------------

/*

  The purpose of the abstract class 'DelaneySymbol' is to provide a
  minimal interface for all sorts of Delaney symbols. It contains
  declarations of fifteen functions listed below which each derived
  class must implement. These functions are pure virtual, thus the
  class 'DelaneySymbol' itself can not be instantiated.

  The interface consists of the following functions:

%%%%% FIX ME! %%%%%

  dim
  is_finite

  idx_valid
  idx_first
  idx_none
  idx_next

  elm_valid
  elm_first
  elm_none
  elm_next

%%%%% \FIX ME! %%%%%

  The four functions 'op_defined', 'op', 'v_defined' and 'v' do the
  obvious things. The function 'v_implied' returns true if a certain v
  value is implied by the given indices, as is the case for "proper"
  Delaney symbols whenever 'abs(i-j) > 1' holds.

  All of these functions are pure virtual, i.e. each concrete class
  must implement all of them.

%%%%% FIX ME! %%%%%

  A couple of utility functions are implemented on top of the
  interface. Derived classes can use them as they are or override them
  by a more suitable or more efficient implementation.

%%%%% \FIX ME! %%%%%

  Here's a little subtlety: all of these functions must terminate
  normally and return a value. In case of invalid input parameters,
  this value must be 'invalid' as well. For example, if the return
  value specifies an element or an index, a value x giving
  'elm_valid(x) = false' or 'idx_valid(x) = false', resp., must be
  chosen. For things like branching indices or sizes, zero is best.
  (But note that zero is sometimes used as a placeholder for infinity,
  so if the difference is important, functions like 'v_defined' and
  'is_finite' must be used to check.)

  A nice example of how the interface works is given by the print
  function.

*/


class DelaneySymbol {

public:
  // useful type names:

  typedef std::vector<int> elm_type;
  typedef int              idx_type;
  typedef size_t           size_type;

  // an abstract class must have a destructor:

  virtual ~DelaneySymbol () = 0;

  // --------------------------------------------------
  // These methods constitute the interface. They must be implemented
  // by any derived class.

  virtual size_type dim () const = 0;
  virtual bool is_finite () const = 0;

  virtual bool      idx_valid (const idx_type&) const = 0;
  virtual idx_type  idx_first () const = 0;
  virtual idx_type  idx_none  () const = 0;
  virtual idx_type  idx_next  (const idx_type&) const = 0;

  virtual bool      elm_valid (const elm_type&) const = 0;
  virtual elm_type  elm_first () const = 0;
  virtual elm_type  elm_none  () const = 0;
  virtual elm_type  elm_next  (const elm_type&) const = 0;

  virtual bool
  op_defined (const idx_type&, const elm_type&) const = 0;
  virtual elm_type
  op (const idx_type&, const elm_type&) const = 0;

  virtual bool
  v_defined (const idx_type&, const idx_type&, const elm_type&) const = 0;
  virtual int
  v (const idx_type&, const idx_type&, const elm_type&) const = 0;
  virtual bool
  v_implied (const idx_type&, const idx_type&) const = 0;

  // --------------------------------------------------
  // Utility functions which are defined on top of the
  // interface. These need only be overwritten when a more efficient
  // implementation is at hand or when the default implementation
  // becomes invalid.

  virtual int
  r(const idx_type& i, const idx_type& j, const elm_type& elm) const;

  virtual int
  m(const idx_type& i, const idx_type& j, const elm_type& elm) const
  {
    return v(i, j, elm) * r(i, j, elm);
  }

  virtual size_type size () const;

  virtual Answer is_complete        () const;
  virtual Answer is_proper          () const;
  virtual Answer is_loopless        () const;
  virtual Answer is_simple          () const;
  virtual Answer is_connected       () const;
  virtual Answer is_weakly_oriented () const;
  virtual Answer is_oriented        () const;

  virtual void print (std::ostream& out) const;
};


DelaneySymbol::~DelaneySymbol()
{
}


int
DelaneySymbol::r
(const idx_type& i, const idx_type& j, const elm_type& elm) const
{
  elm_type D = elm;
  int n = 0;

  do {
    if (op_defined(i, D))
      D = op(i, D);
    if (op_defined(j, D))
      D = op(j, D);
    n++;
  } while (D != elm);
  
  return n;
}


// The following implementations (up to the end of this file) only
// work for finite symbols:

// The following simple function counts the number of elements of a
// Delaney symbol. Assumably, there will be nicer ways to determine
// the size for almost all 'real' symbols.

DelaneySymbol::size_type
DelaneySymbol::size () const
{
  size_type count = 0;

  if (is_finite())
    for (elm_type D = elm_first(); elm_valid(D); D = elm_next(D))
      count++;
  
  return count;
}


// A Delaney symbol is called complete if op and v are defined for all
// legal element-index combinations.

Answer
DelaneySymbol::is_complete() const
{
  if (!is_finite())
    return Maybe;

  idx_type i, j;
  elm_type D;

  for (i = idx_first(); idx_valid(i); i = idx_next(i)) {
    for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
      if (!op_defined(i, D))
	return false;
    }
    for (j = idx_next(i); idx_valid(j); j = idx_next(j)) {
      for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
	if (!v_defined(i, j, D))
	  return false;
      }
    }
  }
  return true;
}


// A Delaney symbol is called proper if for each pair of different
// non-consecutive indices i and j the correponding 'op's commute and
// 'v_defined(i, j, D)' implies 'm(i, j, D) == 2' (see the central
// if-statement below). Note that the straightforward interpretation
// of "non-consecutive" here may be inappropriate for things like
// sections and subsymbols.

Answer
DelaneySymbol::is_proper() const
{
  if (!is_finite())
    return Maybe;

  idx_type i, j;
  elm_type D;

  for (i = idx_first(); idx_valid(i); i = idx_next(i)) {
    for (j = idx_next(idx_next(i)); idx_valid(j); j = idx_next(j)) {
      for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
	if (   op(i, op(j, D)) != op(j, op(i, D))
	    || (v_defined(i, j, D) && m(i, j, D) != 2))
	{
	  return false;
	}
      }
    }
  }
  return true;
}


// A Delaney symbol is loopless if op(i, D) != D for all indices i
// and elements D.

Answer
DelaneySymbol::is_loopless () const
{
  if (!is_finite())
    return Maybe;

  idx_type i;
  elm_type D;

  for (i = idx_first(); idx_valid(i); i = idx_next(i)) {
    for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
      if (!op_defined(i, D) || op(i, D) == D)
	return No;
    }
  }

  return Yes;
}


// We call a Delaney symbol simple if all the branching numbers v
// are defined and equal to 1.

Answer
DelaneySymbol::is_simple () const
{
  if (!is_finite())
    return Maybe;

  idx_type i, j;
  elm_type D;

  for (i = idx_first(); idx_valid(i); i = idx_next(i)) {
    for (j = idx_next(i); idx_valid(j); j = idx_next(j)) {
      for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
	if (!v_defined(i, j, D) || v(i, j, D) != 1)
	  return No;
      }
    }
  }

  return Yes;
}


// A Delaney symbol is connected if there is a path (i.e. a connection
// by a sequence of 'op' relations) between each pair of elements.

Answer
DelaneySymbol::is_connected () const
{
  if (!is_finite())
    return Maybe;

  std::vector<elm_type> queue(size()+1);
  std::set<elm_type> seen;
  size_type head = 1, tail = 1;

  queue[head++] = elm_first();
  seen.insert(elm_first());

  while (tail < head) {
    elm_type D = queue[tail++];

    for (idx_type i = idx_first(); idx_valid(i); i = idx_next(i)) {
      elm_type Di = op(i, D);

      if (seen.count(Di) <= 0) {
	queue[head++] = Di;
	seen.insert(Di);
      }
    }
  }

  return head > size();
}


// A Delaney symbol is oriented if there is a bool valued function d
// on the set of elements such that 'd(op(i,D)) == !D' holds for every
// index i and every element D. Obviously, a symbol can only be
// oriented if it is loopless (see above). We call it weakly oriented,
// if the above holds for all non-loops, i.e. if 'op(i,D) != D'
// always implies 'd(op(i,D)) == !D'.

Answer
DelaneySymbol::is_weakly_oriented () const
{
  if (!is_finite())
    return Maybe;

  std::vector<elm_type> queue(size()+1);
  std::set<elm_type> seen;
  std::map<elm_type, bool> d;

  size_type head, tail;

  for (elm_type seed = elm_first(); elm_valid(seed); seed = elm_next(seed)) {
    if (seen.count(seed) == 0) {
      head = tail = 1;
      queue[head++] = seed;
      seen.insert(seed);
      d[seed] = true;

      while (tail < head) {
	elm_type D = queue[tail++];

	for (idx_type i = idx_first(); idx_valid(i); i = idx_next(i)) {
	  elm_type Di = op(i, D);

	  if (seen.count(Di) == 0) {
	    queue[head++] = Di;
	    seen.insert(Di);
	    d[Di] = !d[D];
	  }
	  else if (Di != D && d[Di] == d[D])
	    return false;
	}
      }
    }
  }

  return true;
}


// Given the above, a symbol is oriented precisely if it is both
// weakly oriented and loopless.

Answer
DelaneySymbol::is_oriented () const
{
  if (!is_finite())
    return Maybe;
  else
    return is_loopless() && is_weakly_oriented();
}

// An output operator for vectors.

std::ostream& operator << (std::ostream& out, const std::vector<int> v)
{
  if (v.size() == 1)
    out << v[0];
  else {
    out << "[";
    for (int i = 0; i < v.size(); i++) {
      if (i > 0)
	out << ",";
      out << v[i];
    }
    out << "]";
  }

  return out;
}

// Here's a generic implementation of the print function.

void
DelaneySymbol::print (std::ostream& out) const
{
  if (!is_finite()) {
    out << "(An infinite Delaney symbol)";
    return;
  }

  idx_type i, j;
  elm_type D;

  for (i = idx_first(); idx_valid(i); i = idx_next(i)) {
    out << "  s(" << i << ") = ";
    std::set<elm_type> seen;

    for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
      if (!(seen.count(D) > 0)) {
	seen.insert(D);
	if (op_defined(i, D)) {
	  elm_type D_i = op(i, D);
	  seen.insert(D_i);
	  
	  if (D_i == D)
	    out << "(" << D << ")";
	  else
	    out << "(" << D << "," << D_i << ")";
	}
      }
    }
    out << std::endl;
  }

  for (i = idx_first(); idx_valid(i); i = idx_next(i)) {
    for (j = idx_next(i); idx_valid(j); j = idx_next(j)) {
      if (!v_implied(i, j)) {
	out << "  v(" << i << "," << j << ") =";
	std::set<elm_type> seen;

	for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
	  if (!seen.count(D) > 0) {
	    if (v_defined(i, j, D))
	      out << "  " << D << "->" << v(i, j, D);

	    elm_type D1 = D;
	    do {
	      if (op_defined(i, D1))
		D1 = op(i, D1);
	      seen.insert(D1);
	      if (op_defined(j, D1))
		D1 = op(j, D1);
	      seen.insert(D1);
	    } while (D1 != D);

	  }
	}
	out << std::endl;
      }
    }
  }
}


inline std::ostream&
operator<< (std::ostream& out, const DelaneySymbol& ds)
{
  ds.print(out);
  return out;
}


// ------------------------------------------------------------------------


#endif //_h_DelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF DelaneySymbol.h --- */
