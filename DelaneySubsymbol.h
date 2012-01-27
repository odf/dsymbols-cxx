/* --------------------------------------------------------------------	*
 *	DelaneySubsymbol.h		07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_DelaneySubsymbol
#define _h_DelaneySubsymbol 1

#include <set>
#include <vector>
#include "DelaneySymbol.h"
#include "IndirectDelaneySymbol.h"

/*

  The class 'DelaneySubsymbol' is another example of an indirect
  Delaney symbol. It is a connected component of a section.

 */


class DelaneySubsymbol : public IndirectDelaneySymbol
{
  typedef std::set<idx_type>::const_iterator idx_iterator;
  typedef std::set<elm_type>::const_iterator elm_iterator;

private:
  std::set<idx_type> theIndices;
  size_type theDim;

  std::set<elm_type> theElements;
  size_type theSize;

public:
  explicit
  DelaneySubsymbol
  (const DelaneySymbol& ds, const std::set<idx_type>& indices, const elm_type& seed);

  ~DelaneySubsymbol() {}

  virtual bool
  is_finite () const { return theBase.is_finite(); }

  virtual idx_type
  idx_none () const { return theBase.idx_none(); }

  virtual elm_type
  elm_none () const { return theBase.elm_none(); }

  size_type dim  () const { return theDim; }
  size_type size () const { return theSize; }

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
  elm_valid (const elm_type& elm) const
  {
    return theElements.count(elm) > 0;
  }

  elm_type
  elm_first () const
  {
    return *theElements.begin();
  }

  elm_type
  elm_next (const elm_type& elm) const
  {
    elm_iterator pos = theElements.find(elm);
    if (pos != theElements.end() && ++pos != theElements.end())
      return *pos;
    else
      return elm_none();
  }

  bool op_defined (const idx_type& idx, const elm_type& elm) const
  {
    return idx_valid(idx) && elm_valid(elm)
      && theBase.op_defined(idx, elm);
  }

  elm_type op (const idx_type& idx, const elm_type& elm) const
  {
    if (op_defined(idx, elm))
      return theBase.op(idx, elm);
    else
      return elm;
  }

  bool v_defined
  (const idx_type& i, const idx_type& j, const elm_type& elm) const
  {
    return idx_valid(i) && idx_valid(j) && elm_valid(elm)
      && theBase.v_defined(i, j, elm);
  }

  int v
  (const idx_type& i, const idx_type& j, const elm_type& elm) const
  {
    if (v_defined(i, j, elm))
      return theBase.v(i, j, elm);
    else
      return 0;
  }

  bool
  v_implied (const idx_type& i, const idx_type& j) const
  {
    if (idx_valid(i) && idx_valid(j))
      return theBase.v_implied(i, j);
    else
      return false;
  }

  Answer is_proper() const;
};


DelaneySubsymbol::DelaneySubsymbol(const DelaneySymbol& ds,
				   const std::set<idx_type>& indices,
				   const elm_type& seed)
  : IndirectDelaneySymbol(ds)
{
  idx_iterator it;
  for (it = indices.begin(); it != indices.end(); it++)
    if (theBase.idx_valid(*it))
      theIndices.insert(*it);
  theDim = theIndices.size() - 1;

  std::vector<elm_type> elm_at_rank(theBase.size()+1);
  size_type head = 1, tail = 1;

  elm_at_rank[head++] = seed;
  theElements.insert(seed);

  while (tail < head) {
    elm_type D = elm_at_rank[tail++];

    for (idx_type i = idx_first(); idx_valid(i); i = idx_next(i)) {
      elm_type Di = theBase.op(i, D);

      if (theElements.count(Di) <= 0) {
	elm_at_rank[head++] = Di;
	theElements.insert(Di);
      }
    }
  }
  theSize = theElements.size();
}


Answer
DelaneySubsymbol::is_proper() const
{
  if (!is_finite())
    return Maybe;

  idx_type i, j;
  elm_type D;

  for (i = theBase.idx_first();
       theBase.idx_valid(i);
       i = theBase.idx_next(i))
  {
    if (idx_valid(i)) {
      for (j = theBase.idx_next(theBase.idx_next(i));
	   theBase.idx_valid(j);
	   j = theBase.idx_next(j))
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


#endif // _h_DelaneySubsymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF DelaneySubsymbol.h --- */
