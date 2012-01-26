/* --------------------------------------------------------------------	*
 *	DelaneyCover.h			11-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_DelaneyCover
#define _h_DelaneyCover 1

#include "DelaneySymbol.h"


template<class NUM>
class DelaneyCover : public IndirectDelaneySymbol<NUM>
{
public:
  typedef IndirectDelaneySymbol<NUM> indirect;

  typedef typename indirect::base_type base_type;
  typedef typename indirect::size_type size_type;
  typedef typename indirect::idx_type idx_type;
  typedef typename indirect::elm_type elm_type;

protected:

  typedef std::vector<NUM> layer_type;

private:

  elm_type
  head(const elm_type& D) const
  {
    size_type len = len_base();
    elm_type tmp(len);

    for (int i = 0; i < len; i++)
      tmp[i] = D[i];

    return tmp;
  }

  layer_type
  tail(const elm_type& D) const
  {
    size_type len = len_layer();
    size_type off = len_base();
    layer_type tmp(len);

    for (int i = 0; i < len; i++)
      tmp[i] = D[i+off];

    return tmp;
  }

  elm_type
  make_elm(const elm_type& D, const layer_type& L) const
  {
    size_type lb = len_base();
    size_type ll = len_layer();
    elm_type tmp(lb+ll);
    int i;

    for (i = 0; i < lb; i++)
      tmp[i] = D[i];
    for (i = 0; i < ll; i++)
      tmp[i+lb] = L[i];

    return tmp;
  }

public:

  // --------------------------------------------------
  // Here's the interface:

  explicit DelaneyCover(const base_type& ds)
    : IndirectDelaneySymbol<NUM>(ds)
  {
  }

  virtual ~DelaneyCover() = 0;

  virtual bool       is_finitely_layered () const = 0;
  virtual size_type  len_base () const = 0;
  virtual size_type  len_layer () const = 0;

  virtual bool       layer_valid (const layer_type&) const = 0;
  virtual layer_type layer_first () const = 0;
  virtual layer_type layer_none () const = 0;
  virtual layer_type layer_next (const layer_type&) const = 0;

  virtual layer_type image_layer (const idx_type&,
				  const elm_type&,
				  const layer_type&)
       const = 0;

  // --------------------------------------------------
  // Here are appropriate redefinitions of the Delaney symbol member
  // functions:

  virtual bool
  is_finite () const {
    return indirect::theBase.is_finite() && is_finitely_layered();
  }

  virtual bool
  elm_valid (const elm_type& D) const {
    return indirect::theBase.elm_valid(head(D)) && layer_valid(tail(D));
  }

  virtual elm_type
  elm_first () const {
    return make_elm(indirect::theBase.elm_first(), layer_first());
  }

  virtual elm_type
  elm_none () const {
    return make_elm(indirect::theBase.elm_none(), layer_none());
  }

  virtual elm_type
  elm_next (const elm_type& D) const {
    if (indirect::theBase.elm_valid(indirect::theBase.elm_next(head(D))))
      return make_elm(indirect::theBase.elm_next(head(D)), tail(D));
    else if (layer_valid(layer_next(tail(D))))
      return make_elm(indirect::theBase.elm_first(), layer_next(tail(D)));
    else
      return elm_none();
  }

  virtual bool
  op_defined (const idx_type& i, const elm_type& D) const {
    return indirect::theBase.op_defined(i, head(D)) && layer_valid(tail(D));
  }

  virtual elm_type
  op (const idx_type&i , const elm_type& D) const {
    if (op_defined(i, D))
      return make_elm(indirect::theBase.op(i, head(D)),
		      image_layer(i, head(D), tail(D)));
    else
      return D;
  }

  virtual bool
  v_defined (const idx_type& i, const idx_type& j, const elm_type& D) const
  {
    return (    indirect::theBase.v_defined(i, j, head(D))
	     && layer_valid(tail(D))
	     && ( indirect::theBase.m(i, j, head(D)) % r(i, j, D) == 0 ) );
  }

  virtual int
  v (const idx_type& i, const idx_type& j, const elm_type& D) const
  {
    if (v_defined(i, j, D))
      return indirect::theBase.m(i, j, head(D)) / r(i, j, D);
    else
      return 0;
  }
};


template<class NUM>
DelaneyCover<NUM>::~DelaneyCover()
{
}


// ------------------------------------------------------------------------


#endif // _h_DelaneyCover

// Local Variables:
// mode:C++
// End:

/* --- EOF DelaneyCover.h --- */
