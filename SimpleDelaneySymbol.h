/* --------------------------------------------------------------------	*
 *	SimpleDelaneySymbol.h		11-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_SimpleDelaneySymbol
#define _h_SimpleDelaneySymbol 1

#include <map>
#include <utility>
#include "DynamicDelaneySymbol.h"


/*

  The class 'SimpleDelaneySymbol' is an example of a concrete class
  derived from 'DelaneySymbol'. It uses int as the data type for
  indices and elements, where elements always range from 1 to size()
  and indices from 0 to dim(). The op and v information is represented
  by STL maps.

  The function 'simple_ds' in this file takes as argument an arbitrary
  object of type 'DelaneySymbol' and produces an isomorphic
  'SimpleDelaneySymbol'. This can fail for two reasons: since in a
  SimpleDelaneySymbol, 'v' can be defined explicitly for consecutive
  indices only, certain 'v' values have to change if the source symbol
  does not follow this rule. An empty symbol is produced if the input
  symbol is infinite.

 */


class SimpleDelaneySymbol : public DynamicDelaneySymbol
{

private:
  size_type theDim;
  size_type theSize;
  typedef std::map< std::pair<int,int>,
		    int,
		    std::less<std::pair<int,int> > > map_type;
  map_type theOp, theV;

  elm_type
  wrap(const int n) const
  {
    elm_type tmp(1);
    tmp[0] = n;
    return tmp;
  }

public:
  SimpleDelaneySymbol(size_type d, size_type sz) : theDim(d), theSize(sz) { }
  ~SimpleDelaneySymbol() {}

  size_type dim  () const { return theDim;  }
  size_type size () const { return theSize; }
  bool is_finite () const { return true; }

  bool
  idx_valid (const idx_type& idx) const
  {
    return idx >= 0 && idx <= theDim;
  }

  idx_type
  idx_first () const
  {
    return 0;
  }

  idx_type
  idx_none () const
  {
    return dim()+1;
  }

  idx_type
  idx_next (const idx_type& idx) const
  {
    if (idx_valid(idx))
      return idx+1;
    else
      return idx_none();
  }


  bool
  elm_valid (const elm_type& elm) const
  {
    return elm[0] >= 1 && elm[0] <= theSize;
  }

  elm_type
  elm_first () const
  {
    return wrap(1);
  }

  elm_type
  elm_none () const
  {
    return wrap(size()+1);
  }

  elm_type
  elm_next (const elm_type& elm) const
  {
    if (elm_valid(elm))
      return wrap(elm[0]+1);
    else
      return elm_none();
  }


private:

  bool
  has(const map_type& which,
      const elm_type& elm, const idx_type& idx) const
  {
    map_type::key_type item(elm[0], idx);

    return which.count(item) > 0;
  }

  const int&
  lookup(const map_type& which,
	 const elm_type& elm, const idx_type& idx) const
  {
    map_type::key_type item(elm[0], idx);

    return (*which.find(item)).second;
  }

  bool
  erase(map_type& which,
	const elm_type& elm, const idx_type& idx)
  {
    map_type::key_type item(elm[0], idx);

    return which.erase(item);
  }

  bool
  insert(map_type& which,
	 const elm_type& elm, const idx_type& idx, const int val);

public:

  bool
  op_defined (const idx_type& idx, const elm_type& elm) const
  {
    return elm_valid(elm) && idx_valid(idx) && has(theOp, elm, idx);
  }

  elm_type
  op (const idx_type& idx, const elm_type& elm) const
  {
    if (elm_valid(elm) && idx_valid(idx) && has(theOp, elm, idx))
      return wrap(lookup(theOp, elm, idx));
    else 
      return elm;
  }

  bool
  v_defined (const idx_type& i, const idx_type& j, const elm_type& elm) const;

  int
  v (const idx_type& i, const idx_type& j, const elm_type& elm) const;

  bool
  v_implied
  (const idx_type& i, const idx_type& j) const;

protected:
  bool
  unset_op_internal(const idx_type& idx, const elm_type& elm);

  bool
  set_op_internal(const idx_type& idx,
		  const elm_type& elm, const elm_type& img);

  bool
  unset_v_internal(const idx_type& i, const idx_type& j, const elm_type& elm);

  bool
  set_v_internal(const idx_type& i, const idx_type& j,
		 const elm_type& elm, const int new_v);

  bool
  add_element_internal(const elm_type& elm);

  bool
  remove_element_internal(const elm_type& elm);

public:
  bool
  set_op(const idx_type& i, const elm_type& elm, const elm_type& img)
  {
    return DynamicDelaneySymbol::set_op(i, elm, img);
  }

  bool
  set_op(const idx_type& i, const int elm, const int img)
  {
    return set_op(i, wrap(elm), wrap(img));
  }

  bool
  unset_op(const idx_type& i, const elm_type& elm)
  {
    return DynamicDelaneySymbol::unset_op(i, elm);
  }

  bool
  unset_op(const idx_type& i, const int elm)
  {
    return unset_op(i, wrap(elm));
  }

  bool
  set_v(const idx_type& i, const idx_type& j, const elm_type& elm, const int v)
  {
    return DynamicDelaneySymbol::set_v(i, j, elm, v);
  }

  bool
  set_v(const idx_type& i, const idx_type& j, const int elm, const int v)
  {
    return set_v(i, j, wrap(elm), v);
  }

  bool
  unset_v(const idx_type& i, const idx_type& j, const elm_type& elm)
  {
    return DynamicDelaneySymbol::unset_v(i, j, elm);
  }

  bool
  unset_v(const idx_type& i, const idx_type& j, const int elm)
  {
    return unset_v(i, j, wrap(elm));
  }

};


// Here are implementations for those member functions which have not
// already been defined within the class declaration:

bool
SimpleDelaneySymbol::insert
(map_type& which, const elm_type& elm, const idx_type& idx, const int val)
{
  map_type::key_type item(elm[0], idx);

  if (has(which, elm, idx))
    return false;
  else {
    which[item] = val;
    return true;
  }
}


bool
SimpleDelaneySymbol::v_defined
(const idx_type& i, const idx_type& j, const elm_type& elm) const
{
  if (elm_valid(elm) && idx_valid(i) && idx_valid(j)) {
    if (j == i - 1)
      return has(theV, elm, j);
    else if (j == i + 1)
      return has(theV, elm, i);
    else
      return true;
  }
  else
    return false;
}

int
SimpleDelaneySymbol::v
(const idx_type& i, const idx_type& j, const elm_type& elm) const
{
  if (elm_valid(elm) && idx_valid(i) && idx_valid(j)) {
    if (j == i - 1) {
      if (has(theV, elm, j))
	return lookup(theV, elm, j);
      else
	return 0;
    }
    else if (j == i + 1) {
      if (has(theV, elm, i))
	return lookup(theV, elm, i);
      else
	return 0;
    }
    else if (j == i)
      return 1;
    else {
      if (op_defined(i, elm) && op_defined(j, elm)
	  && op(i, elm) == op(j, elm))
	{
	  return 2;
	}
      else
	return 1;
    }
  }
  else
    return 0;
}

bool
SimpleDelaneySymbol::v_implied
(const idx_type& i, const idx_type& j) const
{
  if (idx_valid(i) && idx_valid(j))
    return j != i-1 && j != i+1;
  else
    return false;
}


bool
SimpleDelaneySymbol::unset_op_internal
(const idx_type& idx, const elm_type& elm)
{
  if (elm_valid(elm) && idx_valid(idx)) {
    if (has(theOp, elm, idx))
      return erase(theOp, elm, idx);
    else
      return true;
  }
  else
    return false;
}

bool
SimpleDelaneySymbol::set_op_internal
(const idx_type& idx, const elm_type& elm, const elm_type& img)
{
  if (elm_valid(elm) && idx_valid(idx)) {
    if (op_defined(idx, elm) && (op(idx, elm) == img))
      return true;
    else if (has(theOp, elm, idx))
      return false;
    else
      return insert(theOp, elm, idx, img[0]);
  }
  else
    return false;
}

bool
SimpleDelaneySymbol::unset_v_internal
(const idx_type& i, const idx_type& j, const elm_type& elm)
{
  if (elm_valid(elm) && idx_valid(i) && idx_valid(j)) {
    if (j == i-1 && has(theV, elm, j))
      return erase(theV, elm, j);
    else if (j == i+1 && has(theV, elm, i))
      return erase(theV, elm, i);
    else
      return true;
  }
  else
    return false;
}

bool
SimpleDelaneySymbol::set_v_internal
(const idx_type& i, const idx_type& j, const elm_type& elm, const int new_v)
{
  if (elm_valid(elm) && idx_valid(i) && idx_valid(j)) {
    if (v_defined(i, j, elm) && (v(i, j, elm) == new_v))
      return true;
    else if (j == i-1 && !has(theV, elm, j))
      return insert(theV, elm, j, new_v);
    else if (j == i+1 && !has(theV, elm, i))
      return insert(theV, elm, i, new_v);
  }

  return false;
}

bool
SimpleDelaneySymbol::add_element_internal(const elm_type& elm)
{
  if (elm[0] == theSize+1) {
    ++theSize;
    return true;
  }
  else
    return false;
}

bool
SimpleDelaneySymbol::remove_element_internal(const elm_type& elm)
{
  if (elm[0] == theSize && elm[0] > 0) {
    for (idx_type i = 0; i <= theDim-1; i++) {
      unset_op(i, elm);
      unset_v(i, i+1, elm);
    }
    unset_op(theDim, elm);
    --theSize;
    return true;
  }
  return true;
}


// ------------------------------------------------------------------------


SimpleDelaneySymbol
simple_ds(const DelaneySymbol& src)
{
  if (!src.is_finite())
    return SimpleDelaneySymbol(0, 0);

  typedef typename DelaneySymbol::elm_type elm_type_src;
  typedef typename DelaneySymbol::idx_type idx_type_src;

  typedef typename SimpleDelaneySymbol::elm_type elm_type_dst;
  typedef typename SimpleDelaneySymbol::idx_type idx_type_dst;

  SimpleDelaneySymbol tmp(src.dim(), src.size());

  std::map<elm_type_src, elm_type_dst> mapped_elm;
  std::map<idx_type_src, idx_type_dst> mapped_idx;

  elm_type_src D;
  idx_type_src i, i1;

  elm_type_dst D_n;
  idx_type_dst i_n, i1_n;

  for (i = src.idx_first(), i_n = tmp.idx_first();
       src.idx_valid(i) && tmp.idx_valid(i_n);
       i = src.idx_next(i), i_n = tmp.idx_next(i_n))
  {
    mapped_idx[i] = i_n;
  }

  for (D = src.elm_first(), D_n = tmp.elm_first();
       src.elm_valid(D) && tmp.elm_valid(D_n);
       D = src.elm_next(D), D_n = tmp.elm_next(D_n))
  {
    mapped_elm[D] = D_n;
  }


  for (i = src.idx_first(); src.idx_valid(i); i = src.idx_next(i)) {
    i_n = mapped_idx[i];
    for (D = src.elm_first(); src.elm_valid(D); D = src.elm_next(D)) {
      if (src.op_defined(i, D))
	tmp.set_op(i_n, mapped_elm[D], mapped_elm[src.op(i,D)]);
    }
  }

  for (i = src.idx_first(), i1 = src.idx_next(i);
       src.idx_valid(i1); i = i1, i1 = src.idx_next(i1))
  {
    i_n = mapped_idx[i];
    i1_n = mapped_idx[i1];

    for (D = src.elm_first(); src.elm_valid(D); D = src.elm_next(D))
      if (src.v_defined(i, i1, D))
	tmp.set_v(i_n, i1_n, mapped_elm[D], src.v(i, i1, D));
  }

  return tmp;
}


// ------------------------------------------------------------------------

#endif // _h_SimpleDelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF SimpleDelaneySymbol.h --- */
