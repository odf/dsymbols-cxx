/* --------------------------------------------------------------------	*
 *	SectionDelaneySymbol.h		07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_SectionDelaneySymbol
#define _h_SectionDelaneySymbol 1

#include <set>
#include "DelaneySymbol.h"
#include "IndirectDelaneySymbol.h"

/*

  The class 'SectionDelaneySymbol' is a very simple example of an
  indirect Delaney symbol. It uses the same elements as the base
  symbol, but only a subset of the indices.

*/


template<class NUM = int>
class SectionDelaneySymbol : public IndirectDelaneySymbol<NUM>
{
public:
  typedef IndirectDelaneySymbol<NUM> indirect;

  typedef typename indirect::base_type base_type;
  typedef typename indirect::size_type size_type;
  typedef typename indirect::idx_type idx_type;
  typedef typename indirect::elm_type elm_type;

  typedef typename std::set<idx_type>::const_iterator idx_iterator;

private:
  std::set<idx_type> theIndices;
  size_type theDim;

public:
  explicit
  SectionDelaneySymbol(const base_type& ds, const std::set<idx_type>& indices)
    : IndirectDelaneySymbol<NUM>(ds)
  {
    idx_iterator it;
    for (it = indices.begin(); it != indices.end(); it++)
      if (ds.idx_valid(*it))
	theIndices.insert(*it);
    theDim = theIndices.size() - 1;
  }

  ~SectionDelaneySymbol() {}

  virtual bool
  is_finite () const { return indirect::theBase.is_finite(); }

  virtual idx_type
  idx_none () const { return indirect::theBase.idx_none(); }

  virtual elm_type
  elm_first () const { return indirect::theBase.elm_first(); }

  size_type dim () const { return theDim; }

  bool
  idx_valid (const idx_type& i) const
  {
    return theIndices.count(i) > 0;
  }

  idx_type
  idx_first () const
  {
    return *theIndices.begin();
  }

  idx_type
  idx_next (const idx_type& idx) const
  {
    idx_iterator pos = theIndices.find(idx);
    if (pos != theIndices.end() && ++pos != theIndices.end())
      return *pos;
    else
      return idx_none();
  }


  bool
  op_defined (const idx_type& i, const elm_type& D) const {
    return idx_valid(i) && indirect::theBase.op_defined(i, D);
  }

  elm_type
  op (const idx_type& i, const elm_type& D) const {
    if (op_defined(i, D))
      return indirect::theBase.op(i, D);
    else
      return D;
  }

  bool
  v_defined (const idx_type& i, const idx_type& j, const elm_type& D) const
  {
    return idx_valid(i) && idx_valid(j) && indirect::theBase.v_defined(i, j, D);
  }

  int
  v (const idx_type& i, const idx_type& j, const elm_type& D) const {
    if (v_defined(i, j, D))
      return indirect::theBase.v(i, j, D);
    else
      return 0;
  }

  bool
  v_implied (const idx_type& i, const idx_type& j) const
  {
    if (idx_valid(i) && idx_valid(j))
      return indirect::theBase.v_implied(i, j);
    else
      return false;
  }

  Answer is_proper() const;
};


// ------------------------------------------------------------------------


template<class NUM>
Answer
SectionDelaneySymbol<NUM>::is_proper() const
{
  if (!is_finite())
    return Maybe;

  idx_type i, j;
  elm_type D;

  for (i = indirect::theBase.idx_first();
       indirect::theBase.idx_valid(i);
       i = indirect::theBase.idx_next(i))
  {
    if (idx_valid(i)) {
      for (j = indirect::theBase.idx_next(indirect::theBase.idx_next(i));
	   indirect::theBase.idx_valid(j);
	   j = indirect::theBase.idx_next(j))
      {
	if (idx_valid(j)) {
	  for (D = elm_first(); elm_valid(D); D = elm_next(D)) {
	    if (op(i, op(j, D)) != op(j, op(i, D))
		   || (v_defined(i, j, D) && m(i, j, D) != 2))
	    {
	      return false;
	    }
	  }
	}
      }
    }
  }
  return true;
}

// ------------------------------------------------------------------------


#endif // _h_SectionDelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF SectionDelaneySymbol.h --- */
