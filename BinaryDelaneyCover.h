/* --------------------------------------------------------------------	*
 *	BinaryDelaneyCover.h		11-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_BinaryDelaneyCover
#define _h_BinaryDelaneyCover 1

#include "DelaneySymbol.h"
#include "DelaneyCover.h"


class BinaryDelaneyCover : public DelaneyCover
{
private:
  layer_type
  wrap(int n) const
  {
    layer_type tmp(1);
    tmp[0] = n;
    return tmp;
  }

public:

  explicit BinaryDelaneyCover(const DelaneySymbol& ds) : DelaneyCover(ds)
  {
  }

  ~BinaryDelaneyCover() {}

  bool
  is_finitely_layered () const {
    return true;
  }

  size_type
  len_layer () const {
    return 1;
  }

  size_type
  len_base () const {
    return DelaneyCover::theBase.elm_first().size();
  }

  bool
  layer_valid (const layer_type& lay) const {
    return lay[0] == 0 || lay[0] == 1;
  }

  layer_type
  layer_first () const {
    return wrap(0);
  }

  layer_type
  layer_none () const
  {
    return wrap(2);
  }

  layer_type
  layer_next (const layer_type& lay) const
  {
    switch (lay[0]) {
    case 0: return wrap(1);
    default: return layer_none();
    }
  }

  layer_type
  image_layer (const idx_type& i,
	       const std::vector<int>& D,
	       const layer_type& L) const
  {
    if (layer_valid(L))
      return wrap(1 - L[0]);
    else
      return layer_none();
  }
};

// ------------------------------------------------------------------------


#endif // _h_BinaryDelaneyCover

// Local Variables:
// mode:C++
// End:

/* --- EOF BinaryDelaneyCover.h --- */
