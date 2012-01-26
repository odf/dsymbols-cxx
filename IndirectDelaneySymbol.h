/* --------------------------------------------------------------------	*
 *	IndirectDelaneySymbol.h		08-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_IndirectDelaneySymbol
#define _h_IndirectDelaneySymbol 1

#include "DelaneySymbol.h"

/*

  One of the main advantages of using an abstract base class is that
  derived Delaney symbols can be easily realized as 'indirect'
  objects. This is done by storing a reference to an object of type
  'DelaneySymbol' (the base symbol) and letting each interface
  function for the indirect symbol call the corresponding function for
  the base.

  The class 'IndirectDelaneySymbol' is the most simple example
  possible. It's behaviour exactly mimics that of the base
  symbol. This is not very useful by itself, but it makes the
  implementation of other indirect classes like 'DualDelaneySymbol' a
  little easier. In fact, the class is abstract, i.e., it can never be
  instantiated. This is enforced by making the destructor pure
  virtual.

 */


class IndirectDelaneySymbol : public DelaneySymbol
{
protected:
  const DelaneySymbol& theBase;

public:
  explicit IndirectDelaneySymbol(const IndirectDelaneySymbol& ds)
    : theBase(ds)
  {
  }

  explicit IndirectDelaneySymbol(const DelaneySymbol& ds)
    : theBase(ds)
  {
  }

  virtual ~IndirectDelaneySymbol() = 0;

  virtual size_type
  dim () const { return theBase.dim();  }

  virtual bool
  is_finite () const { return theBase.is_finite(); }

  virtual bool
  idx_valid (const idx_type& i) const { return theBase.idx_valid(i); }

  virtual idx_type
  idx_first () const { return theBase.idx_first(); }

  virtual idx_type
  idx_none () const { return theBase.idx_none(); }

  virtual idx_type
  idx_next (const idx_type& i) const { return theBase.idx_next(i); }

  virtual bool
  elm_valid (const elm_type& D) const { return theBase.elm_valid(D); }

  virtual elm_type
  elm_first () const { return theBase.elm_first(); }

  virtual elm_type
  elm_none () const { return theBase.elm_none(); }

  virtual elm_type
  elm_next (const elm_type& D) const { return theBase.elm_next(D); }

  virtual bool
  op_defined (const idx_type& i, const elm_type& D) const {
    return theBase.op_defined(i, D);
  }

  virtual elm_type
  op (const idx_type&i , const elm_type& D) const {
    return theBase.op(i, D);
  }

  virtual bool
  v_defined (const idx_type& i, const idx_type& j, const elm_type& D) const
  {
    return theBase.v_defined(i, j, D);
  }

  virtual int
  v (const idx_type& i, const idx_type& j, const elm_type& D) const {
    return theBase.v(i, j, D);
  }

  virtual bool
  v_implied (const idx_type& i, const idx_type& j) const
  {
    return theBase.v_implied(i, j);
  }
};


IndirectDelaneySymbol::~IndirectDelaneySymbol()
{
}


// ------------------------------------------------------------------------


#endif // _h_IndirectDelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF IndirectDelaneySymbol.h --- */
