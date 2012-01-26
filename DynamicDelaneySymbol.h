/* --------------------------------------------------------------------	*
 *	DynamicDelaneySymbol.h		07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_DynamicDelaneySymbol
#define _h_DynamicDelaneySymbol 1

#include <stddef.h>
#include <set>
#include "Answer.h"
#include "DelaneySymbol.h"


// ------------------------------------------------------------------------

/*

  Here is an extended interface for dynamic, i.e. mutable, Delaney
  symbols, implemented as an abstract class 'DynamicDelaneySymbol'
  which is derived from 'DelaneySymbol'.

  The additional interface functions are:

  unset_op_internal, set_op_internal, unset_v_internal,
  set_v_internal, add_element_internal, remove_element_internal

  Each of these functions returns a boolean value stating whether the
  attempted action was successful or not.

  These functions are 'atomic' in that they only affect single
  elements of the symbol.  Therefore, they are declared protected and
  are not intended to be called directly by a class user.  Instead,
  the corresponding functions unset_op, set_op, ..., should be called.
  These functions are defined for the class DynamicDelaneySymbol and
  use multiple calls of the corresponding '..._internal' functions to
  keep the symbol consistent.

  A class inheriting from DynamicDelaneySymbol need only implement the
  internal functions, but it can alternatively implement the public
  functions, e.g. if it uses a representation of Delaney symbols where
  atomic changes are either unnecessary or impossible.

*/


class DynamicDelaneySymbol : public DelaneySymbol
{
protected:
  virtual bool
  unset_op_internal (const idx_type&, const elm_type&)
  {
    return false;
  }

  virtual bool
  set_op_internal (const idx_type&, const elm_type&, const elm_type&)
  {
    return false;
  }

  virtual bool
  unset_v_internal (const idx_type&, const idx_type&, const elm_type&)
  {
    return false;
  }

  virtual bool
  set_v_internal (const idx_type& i, const idx_type& j,
		  const elm_type& elm, const int new_v)
  {
    return false;
  }

  virtual bool
  add_element_internal (const elm_type&)
  {
    return false;
  }

  virtual bool
  remove_element_internal (const elm_type&)
  {
    return false;
  }

public:
  virtual ~DynamicDelaneySymbol () = 0;

  virtual bool
  unset_op(const idx_type& idx, const elm_type& elm)
  {
    elm_type img = op(idx, elm);
    return ( unset_op_internal(idx, elm)
	     &&
	     unset_op_internal(idx, img) );
  }

  virtual bool
  set_op(const idx_type& idx, const elm_type& elm, const elm_type& img)
  {
    return ( set_op_internal(idx, elm, img)
	     &&
	     set_op_internal(idx, img, elm) );
  }

  virtual bool
  unset_v(const idx_type& i, const idx_type& j, const elm_type& elm)
  {
    elm_type D = elm;

    do {
      if (!unset_v_internal(i, j, D))
	return false;
      if (op_defined(i, D))
	D = op(i, D);
      if (!unset_v_internal(i, j, D))
	return false;
      if (op_defined(j, D))
	D = op(j, D);
    } while (D != elm);

    return true;
  }

  virtual bool
  set_v(const idx_type& i, const idx_type& j,
	const elm_type& elm, const int new_v)
  {
    elm_type D = elm;

    do {
      if (!set_v_internal(i, j, D, new_v))
	return false;
      if (op_defined(i, D))
	D = op(i, D);
      if (!set_v_internal(i, j, D, new_v))
	return false;
      if (op_defined(j, D))
	D = op(j, D);
    } while (D != elm);

    return true;
  }

  virtual bool
  add_element(const elm_type& elm)
  {
    return add_element_internal(elm);
  }

  virtual bool
  remove_element(const elm_type& elm)
  {
    return remove_element_internal(elm);
  }

};


DynamicDelaneySymbol::~DynamicDelaneySymbol()
{
}


// ------------------------------------------------------------------------


#endif //_h_DynamicDelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF DynamicDelaneySymbol.h --- */
