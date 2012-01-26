/* --------------------------------------------------------------------	*
 *	Bag.h				24-sep-1998  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _h_Bag
#define _h_Bag

#include <deque>
#include <vector>
#include <algorithm>


template <class T, class Sequence = std::deque<T> >
class Stack {
  Sequence c;
public:
  typedef typename Sequence::value_type value_type;

  bool empty() const { return c.empty(); }

  void push(const value_type& x) { c.push_back(x); }

  value_type
  pop() {
    value_type tmp = c.back();
    c.pop_back();
    return tmp;
  }
};


template <class T, class Sequence = std::deque<T> >
class Queue {
  Sequence c;
public:
  typedef typename Sequence::value_type value_type;

  bool empty() const { return c.empty(); }

  void push(const value_type& x) { c.push_back(x); }

  value_type
  pop() {
    value_type tmp = c.front();
    c.pop_front();
    return tmp;
  }
};


template <class T, class Sequence = std::vector<T>, 
          class Compare = std::less<typename Sequence::value_type> >
class  PriorityQueue {
  Sequence c;
  Compare comp;
public:
  typedef typename Sequence::value_type value_type;

  PriorityQueue() : c() {}
  explicit PriorityQueue(const Compare& x) :  c(), comp(x) {}

  bool empty() const { return c.empty(); }

  void
  push(const value_type& x) {
    c.push_back(x); 
    std::push_heap(c.begin(), c.end(), comp);
  }

  value_type
  pop() {
    value_type tmp = c.front();
    std::pop_heap(c.begin(), c.end(), comp);
    c.pop_back();
    return tmp;
  }
};


#endif /* _h_Bag */

// Local Variables:
// mode:C++
// End:

/* --- EOF Bag.h --- */
