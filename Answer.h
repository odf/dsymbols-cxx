// This may look like C code, but it is really -*- C++ -*-

/* --------------------------------------------------------------------	*
 *	Answer.h			16-sep-1998  by ODF		*
 * --------------------------------------------------------------------	*/


#ifndef _Answer_h
#define _Answer_h 1

#include <list>
#include <stddef.h>

// ------------------------------------------------------------------------

class Answer {
public:
  enum val { No = 0, Yes, Maybe };
  enum val theVal;
public:
  Answer(val p = No) { theVal = p; }
  Answer(bool p) { if (p) theVal = Yes; else theVal = No; }

  friend std::ostream& operator << (std::ostream&, const Answer);

  operator bool () const
  {
    if (theVal == Yes)
      return 1;
    else
      return 0;
  }

  Answer
  operator ! () const {
    switch (theVal) {
    case Answer::Yes:   return No;
    case Answer::No:    return Yes;
    case Answer::Maybe: return Maybe;
    }
    return No;
  }

  Answer
  operator & (Answer rval) const {
    switch (theVal) {
    case Answer::Yes:	return rval;
    case Answer::No:	return No;
    case Answer::Maybe:	return (rval == Answer(No)) ? No : Maybe;
    }
    return No;
  }

  Answer
  operator | (Answer rval) const {
    switch (theVal) {
    case Answer::Yes:	return Yes;
    case Answer::No:	return rval;
    case Answer::Maybe:	return (rval == Answer(Yes)) ? Yes : Maybe;
    }
    return No;
  }

  bool
  operator == (Answer rval) const {
    return theVal == rval.theVal;
  }
};

static const Answer Yes   = Answer::Yes;
static const Answer No    = Answer::No;
static const Answer Maybe = Answer::Maybe;

inline std::ostream&
operator << (std::ostream& out, const Answer p)
{
  switch (p.theVal) {
  case Answer::Yes:   out << "Yes"; break;
  case Answer::No:    out << "No"; break;
  case Answer::Maybe: out << "Maybe"; break;
  }
  return out;
}

// ------------------------------------------------------------------------

#endif /* !_Answer_h */

/* --- EOF Answer.h --- */
