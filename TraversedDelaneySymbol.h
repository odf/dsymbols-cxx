/* --------------------------------------------------------------------	*
 *	TraversedDelaneySymbol.h	07-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_TraversedDelaneySymbol
#define _h_TraversedDelaneySymbol 1

#include "DelaneySymbol.h"
#include "IndirectDelaneySymbol.h"
#include "Bag.h"


template<class NUM = int, class BT = Queue<std::pair<NUM,std::vector<NUM> > > >
class TraversedDelaneySymbol : public IndirectDelaneySymbol<NUM>
{
public:
  typedef IndirectDelaneySymbol<NUM> indirect;

  typedef typename indirect::base_type base_type;
  typedef typename indirect::size_type size_type;
  typedef typename indirect::idx_type idx_type;
  typedef typename indirect::elm_type elm_type;

  typedef BT bag_type;
  typedef TraversedDelaneySymbol<NUM,BT> sym_type;

private:
  std::map<elm_type, size_type> elm_to_pos;
  std::vector<elm_type> pos_to_elm;
  std::vector<idx_type> pos_to_idx;
  elm_type seed;
  size_type pos;

  bag_type seen_edges;
  const elm_type first_seed;

  size_type advance_to(const elm_type& D, bool one_more);
  
public:
  explicit
  TraversedDelaneySymbol(const base_type& ds)
    : IndirectDelaneySymbol<NUM> (ds),
      pos_to_elm(ds.size()+1, ds.elm_none()),
      pos_to_idx(ds.size()+1, ds.idx_none()),
      pos(0),
      first_seed(ds.elm_first())
  {
  }

  TraversedDelaneySymbol(const base_type& ds, const elm_type& D)
    : IndirectDelaneySymbol<NUM> (ds),
      pos_to_elm(ds.size()+1, ds.elm_none()),
      pos_to_idx(ds.size()+1, ds.idx_none()),
      pos(0),
      first_seed(D)
  {
  }

  TraversedDelaneySymbol(const base_type& ds, const bag_type& bag)
    : IndirectDelaneySymbol<NUM> (ds),
      pos_to_elm(ds.size()+1, ds.elm_none()),
      pos_to_idx(ds.size()+1, ds.idx_none()),
      pos(0),
      seen_edges(bag),
      first_seed(ds.elm_first())
  {
    while (!seen_edges.empty())
      seen_edges.pop();
  }

  TraversedDelaneySymbol(const base_type& ds,
			 const bag_type& bag,
			 const elm_type& D    )
    : IndirectDelaneySymbol<NUM> (ds),
      pos_to_elm(ds.size()+1, ds.elm_none()),
      pos_to_idx(ds.size()+1, ds.idx_none()),
      pos(0),
      seen_edges(bag),
      first_seed(D)
  {
    while (!seen_edges.empty())
      seen_edges.pop();
  }

  ~TraversedDelaneySymbol() {}

  virtual idx_type
  idx_none () const { return indirect::theBase.idx_none(); }

  virtual idx_type
  idx_first () const { return indirect::theBase.idx_first(); }

  virtual elm_type
  elm_none () const { return indirect::theBase.elm_none(); }

  elm_type
  elm_first () const {
    if (indirect::theBase.elm_valid(first_seed))
      return first_seed;
    else
      return indirect::theBase.elm_first();
  }

  elm_type
  elm_next (const elm_type& D) const {
    if (elm_valid(D))
      return pos_to_elm[(const_cast<sym_type*>(this))->advance_to(D, true)];
    else
      return elm_none();
  }

  idx_type
  idx_back_edge (const elm_type& D) const {
    if (elm_valid(D))
      return pos_to_idx[(const_cast<sym_type*>(this))->advance_to(D, false)];
    else
      return idx_none();
  }

  void
  print (std::ostream& out) const;
};


template<class NUM, class BT>
typename TraversedDelaneySymbol<NUM,BT>::size_type
TraversedDelaneySymbol<NUM,BT>::advance_to
(const elm_type& elm, bool one_more)
{
  bool last_step = false;

  if (elm_to_pos.count(elm) > 0) {
    size_type p_elm = elm_to_pos[elm];

    if (one_more) {
      if (pos > p_elm)
	return p_elm+1;
      else
	last_step = true;
    }
    else
      return p_elm;
  }

  while (true) {
    if (seen_edges.empty()) {
      if (pos == 0)
	seed = elm_first();
      else
	seed = indirect::theBase.elm_next(seed);

      while (elm_to_pos.count(seed) > 0)
	seed = indirect::theBase.elm_next(seed);

      if (indirect::theBase.elm_valid(seed))
	seen_edges.push(make_pair(idx_none(), seed));
      else
	return 0;
    }

    while (!seen_edges.empty()) {
      std::pair<idx_type, elm_type> p = seen_edges.pop();
      idx_type i = p.first;
      elm_type D = p.second;

      if (idx_valid(i))
	D = op(i, D);

      if (elm_to_pos.count(D) == 0) {
	++pos;
	pos_to_elm[pos] = D;
	pos_to_idx[pos] = i;
	elm_to_pos[D] = pos;

	for (idx_type j = idx_first(); idx_valid(j); j = idx_next(j))
	  seen_edges.push(make_pair(j, D));

	if (last_step)
	  return pos;
	else if (D == elm) {
	  if (one_more)
	    last_step = true;
	  else
	    return pos;
	}
      }
    }
  }
}


template<class NUM, class BT>
void
TraversedDelaneySymbol<NUM,BT>::print (std::ostream& out) const
{
  this->DelaneySymbol<NUM>::print(out);

  out << "  traversal:" << std::endl;

  for (elm_type D = elm_first(); elm_valid(D); D = elm_next(D)) {
    idx_type i = idx_back_edge(D);
    if (idx_valid(i))
      out << "    (" << D << "<-" << i << "->" << op(i, D) << ')' << std::endl;
    else
      out << "    (" << D << ')' << std::endl;
  }
}


// ------------------------------------------------------------------------


#endif // _h_TraversedDelaneySymbol

// Local Variables:
// mode:C++
// End:

/* --- EOF TraversedDelaneySymbol.h --- */
