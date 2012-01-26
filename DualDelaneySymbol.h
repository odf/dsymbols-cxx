/* --------------------------------------------------------------------	*
 *	DualDelaneySymbol.h		07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_DualDelaneySymbol
#define _h_DualDelaneySymbol 1

#include <map>
#include <vector>
#include "DelaneySymbol.h"
#include "IndirectDelaneySymbol.h"

/*

  The class 'DualDelaneySymbol' is a very simple example of an
  indirect Delaney symbol. It uses the same lists of elements and
  indices as the base symbol, but maps each index to the corresponding
  one from the reversed index list before accessing the base symbol.

 */


template<class NUM = int>
class DualDelaneySymbol : public IndirectDelaneySymbol<NUM>
{
public:
  typedef IndirectDelaneySymbol<NUM> indirect;

  typedef typename indirect::base_type base_type;
  typedef typename indirect::size_type size_type;
  typedef typename indirect::idx_type idx_type;
  typedef typename indirect::elm_type elm_type;

private:
  std::map<idx_type, idx_type> rev;

  void     map_indices ();

  idx_type
  reverse_idx (const idx_type& idx) const
  {
    if (indirect::theBase.idx_valid(idx))
      return (*rev.find(idx)).second;
    else
      return idx;
  }

public:
  explicit
  DualDelaneySymbol(const DualDelaneySymbol& ds)
    : IndirectDelaneySymbol<NUM> ((const DelaneySymbol<NUM>&) ds)
  {
    map_indices();
  }

  explicit
  DualDelaneySymbol(const base_type& ds)
    : IndirectDelaneySymbol<NUM> (ds)
  {
    map_indices();
  }

  ~DualDelaneySymbol() {}

  bool op_defined (const idx_type& idx, const elm_type& elm) const
  {
    return indirect::theBase.op_defined(reverse_idx(idx), elm);
  }

  elm_type op (const idx_type& idx, const elm_type& elm) const
  {
    return indirect::theBase.op(reverse_idx(idx), elm);
  }

  bool v_defined
  (const idx_type& i, const idx_type& j, const elm_type& elm) const
  {
    return indirect::theBase.v_defined(reverse_idx(i), reverse_idx(j), elm);
  }

  int v
  (const idx_type& i, const idx_type& j, const elm_type& elm) const
  {
    return indirect::theBase.v(reverse_idx(i), reverse_idx(j), elm);
  }
};


template<class NUM>
void
DualDelaneySymbol<NUM>::map_indices()
{
  std::vector<idx_type> tab;

  for (idx_type idx = indirect::theBase.idx_first();
       indirect::theBase.idx_valid(idx);
       idx = indirect::theBase.idx_next(idx))
  {
    tab.push_back(idx);
  }
  int n = tab.size();

  for (int i = 0; i < n; i++)
    rev[tab[i]] = tab[n-1-i];
}


// ------------------------------------------------------------------------


#endif // _h_DualDelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF DualDelaneySymbol.h --- */
