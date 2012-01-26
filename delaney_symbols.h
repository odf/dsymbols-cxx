/* --------------------------------------------------------------------	*
 *	delaney_symbols.h		07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_delaney_symbols
#define _h_delaney_symbols 1

#include <stddef.h>
#include <set>
#include <vector>

#include "Answer.h"
#include "Bag.h"
#include "IPBag.h"
#include "Integer.h"
#include "Rational.h"

#include "DelaneySymbol.h"
#include "DynamicDelaneySymbol.h"
#include "SimpleDelaneySymbol.h"
#include "IndirectDelaneySymbol.h"
#include "DualDelaneySymbol.h"
#include "SectionDelaneySymbol.h"
#include "DelaneySubsymbol.h"
#include "TraversedDelaneySymbol.h"
#include "DelaneyCover.h"
#include "BinaryDelaneyCover.h"


// ------------------------------------------------------------------------

// The following function returns the curvature of a connected
// twodimensional Delaney symbol as a pair K of Integers, where the
// actual value is defined as K.first/K.second. For non-connected or
// higher-dimensional symbols, the curvature of the first
// two-dimensional subsymbol is computed.

template<class NUM>
Rational
curvature(const DelaneySymbol<NUM>& ds)
{
  typedef typename DelaneySymbol<NUM>::elm_type elm_type;
  typedef typename DelaneySymbol<NUM>::idx_type idx_type;
  typedef typename DelaneySymbol<NUM>::size_type size_type;

  elm_type D;
  idx_type i, j;

  std::set<idx_type> indices;
  indices.insert(i = ds.idx_first());
  indices.insert(i = ds.idx_next(i));
  indices.insert(i = ds.idx_next(i));

  DelaneySubsymbol<NUM> sub(ds, indices, ds.elm_first());

  if (!sub.is_finite())
    return Rational(0, 0);

  Rational K(-static_cast<long>(sub.size()), 1);

  for (i = sub.idx_first(); sub.idx_valid(i); i = sub.idx_next(i)) {
    for (j = sub.idx_next(i); sub.idx_valid(j); j = sub.idx_next(j)) {
      std::set<elm_type> seen;

      for (D = sub.elm_first(); sub.elm_valid(D); D = sub.elm_next(D)) {
	if (seen.count(D) == 0) {
	  elm_type D1 = D;
	  int v = 1, s = 2;

	  if (sub.v_defined(i, j, D) && sub.v(i, j, D) > 0)
	    v = sub.v(i, j, D);

	  do {
	    if (sub.op_defined(i, D1) && sub.op(i, D1) != D1) {
	      D1 = sub.op(i, D1);
	      seen.insert(D1);
	    }
	    else
	      s = 1;
	    if (sub.op_defined(j, D1) && sub.op(j, D1) != D1) {
	      D1 = sub.op(j, D1);
	      seen.insert(D1);
	    }
	    else
	      s = 1;
	  } while (D1 != D);

	  K += Rational(s, v);
	}
      }
    }
  }

  return K;
}


// ------------------------------------------------------------------------

// Here's a function that returns a vector of representatives for the
// connected components of a Delaney symbol.

template<class NUM>
std::vector<typename DelaneySymbol<NUM>::elm_type>
RepresentativesComponents(const TraversedDelaneySymbol<NUM>& ds)
{
  typename DelaneySymbol<NUM>::elm_type D;
  std::vector<typename DelaneySymbol<NUM>::elm_type> reps;

  for (D = ds.elm_first(); ds.elm_valid(D); D = ds.elm_next(D))
    if (!ds.idx_valid(ds.idx_back_edge(D)))
	reps.push_back(D);

  return reps;
}

template<class NUM>
std::vector<typename DelaneySymbol<NUM>::elm_type>
RepresentativesComponents(const DelaneySymbol<NUM>& ds)
{
  return RepresentativesComponents(TraversedDelaneySymbol<NUM>(ds));
}


// ------------------------------------------------------------------------


#endif // _h_delaney_symbols

// Local Variables:
// mode:C++
// End:

/* --- EOF delaney_symbols.h --- */
