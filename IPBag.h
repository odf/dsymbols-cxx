/* --------------------------------------------------------------------	*
 *	IPBag.h				11-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_IPBag
#define _h_IPBag

#include <map>
#include "Bag.h"


template <class NUM>
class IPBag {
public:
  typedef std::pair<NUM, std::vector<NUM> > value_type;
  typedef NUM                               idx_type;
private:
  idx_type                                  special_a, special_b;
  Stack<value_type>                         st_a, st_b;
  std::map<idx_type, Queue<value_type> >    qu;
public:
  IPBag(const idx_type& idx_a = 0, const idx_type& idx_b = 1)
    : special_a(idx_a),
      special_b(idx_b)
  {
  }

  bool empty() const {
    return st_a.empty() && st_b.empty() && qu.empty();
  }

  void push(const value_type& x) {
    if (x.first == special_a)
      st_a.push(x);
    else if (x.first == special_b)
      st_b.push(x);
    else
      qu[x.first].push(x);
  }

  value_type
  pop() {
    if (!st_a.empty())
      return st_a.pop();
    else if (!st_b.empty())
      return st_b.pop();
    else {
      value_type tmp = qu.begin()->second.pop();
      if (qu.begin()->second.empty())
	qu.erase(qu.begin());
      return tmp;
    }
  }
};

#endif /* _h_IPBag */

// Local Variables:
// mode:C++
// End:

/* --- EOF IPBag.h --- */
