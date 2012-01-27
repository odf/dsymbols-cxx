/* --------------------------------------------------------------------	*
 *	testds.cc			11-apr-1999  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#include <iostream>
#include <set>
#include "Answer.h"
#include "delaney_symbols.h"

using namespace std;

#define CHECK(ds, prop) \
  { \
    Answer is = (ds).is_##prop(); \
    if (is == Yes) \
      cout << ' ' << #prop; \
    else if (is == Maybe) \
      cout << ' ' << #prop << '?'; \
  }

void
testanswer()
{
  Answer yep(Yes);
  Answer nope(false);
  Answer dunno = Maybe;

  cout << "Testing class Answer:" << endl;

  cout << "  " << yep   << ( yep  ? " is" : " isn't") << " true." << endl;
  cout << "  " << yep   << (!yep  ? " is" : " isn't") << " false." << endl;
  cout << "  " << nope  << ( nope ? " is" : " isn't") << " true." << endl;
  cout << "  " << nope  << (!nope ? " is" : " isn't") << " false." << endl;
  cout << "  " << dunno << ( dunno? " is" : " isn't") << " true." << endl;
  cout << "  " << dunno << (!dunno? " is" : " isn't") << " false." << endl;
  cout << endl;

  cout << yep << " & " << yep << " = " << (yep & yep)<< endl;
  cout << yep << " & " << nope << " = " << (yep & nope) << endl;
  cout << yep << " & " << dunno << " = " << (yep & dunno) << endl;
  cout << nope << " & " << yep << " = " << (nope & yep) << endl;
  cout << nope << " & " << nope << " = " << (nope & nope) << endl;
  cout << nope << " & " << dunno << " = " << (nope & dunno) << endl;
  cout << dunno << " & " << yep << " = " << (dunno & yep) << endl;
  cout << dunno << " & " << nope << " = " << (dunno & nope) << endl;
  cout << dunno << " & " << dunno << " = " << (dunno & dunno) << endl;
  cout << endl;

  cout << yep << " | " << yep << " = " << (yep | yep) << endl;
  cout << yep << " | " << nope << " = " << (yep | nope) << endl;
  cout << yep << " | " << dunno << " = " << (yep | dunno) << endl;
  cout << nope << " | " << yep << " = " << (nope | yep) << endl;
  cout << nope << " | " << nope << " = " << (nope | nope) << endl;
  cout << nope << " | " << dunno << " = " << (nope | dunno) << endl;
  cout << dunno << " | " << yep << " = " << (dunno | yep) << endl;
  cout << dunno << " | " << nope << " = " << (dunno | nope) << endl;
  cout << dunno << " | " << dunno << " = " << (dunno | dunno) << endl;
  cout << endl;

  cout << yep << " == " << yep << " = " << (yep == yep) << endl;
  cout << yep << " == " << nope << " = " << (yep == nope) << endl;
  cout << yep << " == " << dunno << " = " << (yep == dunno) << endl;
  cout << nope << " == " << yep << " = " << (nope == yep) << endl;
  cout << nope << " == " << nope << " = " << (nope == nope) << endl;
  cout << nope << " == " << dunno << " = " << (nope == dunno) << endl;
  cout << dunno << " == " << yep << " = " << (dunno == yep) << endl;
  cout << dunno << " == " << nope << " = " << (dunno == nope) << endl;
  cout << dunno << " == " << dunno << " = " << (dunno == dunno) << endl;
  cout << endl;
}


void
showsymbol(const DelaneySymbol& ds)
{
  cout << ds << endl;
  cout << "  properties: ";
  CHECK(ds, complete);
  CHECK(ds, proper);
  CHECK(ds, loopless);
  CHECK(ds, simple);
  CHECK(ds, connected);
  CHECK(ds, weakly_oriented);
  CHECK(ds, oriented);
  cout << endl ;

  DelaneySymbol::elm_type D = ds.elm_first();
  DelaneySymbol::idx_type i = ds.idx_first(), j = ds.idx_next(i);

  cout << "  v(" << i << ", " << j << ", " << D << ") = "
       << ds.v(i,j,D) << endl;
  cout << "  r(" << i << ", " << j << ", " << D << ") = "
       << ds.r(i,j,D) << endl;
  cout << "  m(" << i << ", " << j << ", " << D << ") = "
       << ds.m(i,j,D) << endl;
  cout << "  curvature = " << curvature(ds) << endl;
  cout << endl;
}


int
main()
{
  testanswer();

  SimpleDelaneySymbol ds(2,8);

  ds.set_op(0,1,2);
  ds.set_op(0,3,4);
  ds.set_op(0,5,6);
  ds.set_op(0,7,8);

  ds.set_op(1,1,8);
  ds.set_op(1,2,3);
  ds.set_op(1,4,5);
  ds.set_op(1,6,7);

  ds.set_op(2,1,5);
  ds.set_op(2,2,6);
  ds.set_op(2,3,8);
  ds.set_op(2,4,7);

  ds.set_v(0,1,1,1);
  ds.set_v(1,2,1,1);

  cout << "Testing class SimpleDelaneySymbol:" << endl;
  cout << "  ds:" << endl;
  showsymbol(ds);

  ds.unset_op(2, 1);
  ds.unset_op(2, 2);
  cout << "after ds.unset_op(2, 1) an ds.unset_op(2, 2):" << endl;
  showsymbol(ds);

  ds.unset_v(0, 1, 1);
  cout << "after ds.unset_v(0, 1, 1):" << endl;
  showsymbol(ds);

  ds.set_op(2, 1, 2);
  ds.set_op(2, 5, 5);
  cout << "after ds.set_op(2, 1, 2) and ds.set_op(2, 5, 5):" << endl;
  showsymbol(ds);

  ds.set_op(2, 6, 6);
  cout << "after ds.set_op(2, 6, 6):" << endl;
  showsymbol(ds);

  ds.set_v(0, 1, 1, 3);
  cout << "after ds.set_v(0, 1, 1, 3):" << endl;
  showsymbol(ds);

  DualDelaneySymbol dual(ds);
  cout << "dual:" << endl;
  showsymbol(dual);

  DualDelaneySymbol ddual(dual);
  cout << "dual of dual:" << endl;
  showsymbol(ddual);

  SimpleDelaneySymbol simp = simple_ds(ddual);
  cout << "converted back to simple symbol:" << endl;
  showsymbol(simp);

  BinaryDelaneyCover cov(ds);
  cout << "binary cover of ds:" << endl;
  showsymbol(cov);

  BinaryDelaneyCover ccov(cov);
  cout << "second order cover:" << endl;
  showsymbol(ccov);

  simp = simple_ds(ccov);
  cout << "converted to simple symbol:" << endl;
  showsymbol(simp);

  set<int> indices;
  indices.insert(2);
  indices.insert(0);
  indices.insert(3);
  indices.insert(0);
  indices.insert(-1);

  SectionDelaneySymbol sec(cov, indices);
  cout << "section of binary cover:" << endl;
  showsymbol(sec);

  simp = simple_ds(sec);
  cout << "converted to simple symbol:" << endl;
  showsymbol(simp);

  SimpleDelaneySymbol copy(ds);
  cout << "copy of ds:" << endl;
  showsymbol(copy);

  DelaneySubsymbol sub(ds, indices, ds.elm_first());
  cout << "subsymbol of ds:" << endl;
  showsymbol(sub);

  DelaneySubsymbol subcov(cov, indices, cov.elm_first());
  cout << "subsymbol of cov:" << endl;
  showsymbol(subcov);

  ds.unset_op(2, 2);
  ds.unset_op(2, 5);

  cout << "subsymbol after ds.unset_op(2, 2) and ds.unset_op(2, 5):" << endl;
  showsymbol(sub);

  cout << "subcov after ds.unset_op(2, 2) and ds.unset_op(2, 5):" << endl;
  showsymbol(subcov);

  cout << "copy should stay the same:" << endl;
  showsymbol(copy);

  // --------------------

  SimpleDelaneySymbol ds3(3,10);

  ds3.set_op(0, 1, 2);
  ds3.set_op(0, 3, 4);
  ds3.set_op(0, 5, 6);
  ds3.set_op(0, 7, 8);
  ds3.set_op(0, 9, 10);
  ds3.set_op(1, 1, 7);
  ds3.set_op(1, 2, 8);
  ds3.set_op(1, 3, 5);
  ds3.set_op(1, 4, 6);
  ds3.set_op(1, 9, 10);
  ds3.set_op(2, 1, 9);
  ds3.set_op(2, 2, 10);
  ds3.set_op(2, 3, 4);
  ds3.set_op(2, 7, 7);
  ds3.set_op(2, 8, 8);
  ds3.set_op(3, 1, 3);
  ds3.set_op(3, 2, 4);
  ds3.set_op(3, 8, 6);
  ds3.set_op(3, 7, 5);
  ds3.set_op(3, 9, 10);

  cout << "Simple symbol ds3:" << endl;
  showsymbol(ds3);

  TraversedDelaneySymbol<> trav(ds3);
  cout << "Breadth first traversal of ds3:" << endl;
  showsymbol(trav);

  vector<int> start;
  start.push_back(8);

  TraversedDelaneySymbol<Stack<pair<int, vector<int> > > >
    travd(ds3, start);
  cout << "Depth first traversal of ds3 starting at 8:" << endl;
  showsymbol(travd);

  typedef pair<int, vector<int> > edge;

  TraversedDelaneySymbol<PriorityQueue<edge, vector<edge>, greater<edge> > >
    travp(ds3);
  cout << "Priority traversal of ds3:" << endl;
  showsymbol(travp);

  TraversedDelaneySymbol<IPBag<int> > travi(ds3);
  cout << "Index priority traversal of ds3:" << endl;
  showsymbol(travi);

  SimpleDelaneySymbol stravi = simple_ds(travi);
  cout << "Converted to simple:" << endl;
  showsymbol(stravi);

  set<int> indi;
  indi.insert(0);
  indi.insert(1);
  SectionDelaneySymbol section(ds3, indi);
  cout << "Section for index set (0,1) of ds3:" << endl;
  showsymbol(section);

  vector<vector<int> > reps = RepresentativesComponents(section);
  cout << "Representatives of (0,1) orbits: ";
  for (vector<vector<int> >::const_iterator it = reps.begin();
       it != reps.end();
       it++)
    cout << ' ' << *it;
  cout << endl;

  return 0;
}


/*
** Local Variables:
** mode:C++
** compile-command: "make -k testds"
** End:
*/

/* --- EOF testds.cc --- */
