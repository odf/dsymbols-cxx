// This may look like C code, but it is really -*- C++ -*-

/* --------------------------------------------------------------------	*
 *	Rational.h			17-sep-1998  by ODF		*
 *                               Revised 2012/01/26  by ODF             *
 * --------------------------------------------------------------------	*/


#ifndef _Rational_h
#define _Rational_h 1


#include <stddef.h>
#include "gmp.h"
#include "Integer.h"


// ------------------------------------------------------------------------

/*

  The class 'Rational' provides a simple wrapper for most of the
  Rational routines in the GNU Multiple Precision Arithmetic Library.

*/


class Rational
{
private:
  mpq_t rep;
public:
  Rational() {
    mpq_init(rep);
  }

  Rational(const Rational& n) {
    mpq_init(rep);
    mpq_set(rep, n.rep);
  }

  Rational(const Integer& num, const Integer& den = 1L) {
    mpq_init(rep);
    mpq_set_z(rep, num.rep);
    if (den != 1) {
      mpq_set_den(rep, den.rep);
      mpq_canonicalize(rep);
    }
  }

  Rational(int num, unsigned int den = 1) {
    mpq_init(rep);
    mpq_set_si(rep,
	       static_cast<long>(num),
	       static_cast<unsigned long>(den));
    mpq_canonicalize(rep);
  }

  Rational(unsigned int num, unsigned int den = 1) {
    mpq_init(rep);
    mpq_set_ui(rep,
	       static_cast<unsigned long>(num),
	       static_cast<unsigned long>(den));
    mpq_canonicalize(rep);
  }

  Rational(long num, unsigned long den = 1L) {
    mpq_init(rep);
    mpq_set_si(rep, num, den);
    mpq_canonicalize(rep);
  }

  Rational(unsigned long num, unsigned long den = 1L) {
    mpq_init(rep);
    mpq_set_ui(rep, num, den);
    mpq_canonicalize(rep);
  }

  ~Rational() {
    mpq_clear(rep);
  }

  const Rational&
  operator = (const Rational& n) {
    mpq_set(rep, n.rep); return *this;
  }


// extract numerator and denominator

  Integer
  num () const {
    Integer r;
    mpq_get_num(r.rep, rep);
    return r;
  }

  Integer
  den () const {
    Integer r;
    mpq_get_den(r.rep, rep);
    return r;
  }

// assignment-based operations

  const Rational&
  operator += (const Rational& op) {
    mpq_add(rep, rep, op.rep); return *this;
  }

  const Rational&
  operator -= (const Rational& op) {
    mpq_sub(rep, rep, op.rep); return *this;
  }

  const Rational&
  operator *= (const Rational& op) {
    mpq_mul(rep, rep, op.rep); return *this;
  }

  const Rational&
  operator /= (const Rational& op) {
    mpq_div(rep, rep, op.rep); return *this;
  }


// Comparison

  int
  compare(const Rational& rop) const {
    return mpq_cmp(rep, rop.rep);
  }

  friend bool
  operator == (const Rational& lop, const Rational& rop) {
    return mpq_equal (lop.rep, rop.rep);
  }

  friend bool
  operator != (const Rational& lop, const Rational& rop) {
    return !mpq_equal (lop.rep, rop.rep);
  }


// Arithmetic operators

  Rational
  operator -  () const {
    Rational r; mpq_neg(r.rep, rep); return r;
  }

  friend Rational
  operator +  (const Rational& lop, const Rational& rop) {
    Rational r; mpq_add(r.rep, lop.rep, rop.rep); return r;
  }

  friend Rational
  operator -  (const Rational& lop, const Rational& rop) {
    Rational r; mpq_sub(r.rep, lop.rep, rop.rep); return r;
  }

  friend Rational
  operator *  (const Rational& lop, const Rational& rop) {
    Rational r; mpq_mul(r.rep, lop.rep, rop.rep); return r;
  }

  friend Rational
  operator /  (const Rational& lop, const Rational& rop) {
    Rational r; mpq_div(r.rep, lop.rep, rop.rep); return r;
  }

// miscellaneous functions

  // sign

  int
  sgn() const {
    return mpq_sgn(rep);
  }

  // conversion

  double
  get_double() const {
    return mpq_get_d(rep);
  }
};


// ------------------------------------------------------------------------

// Comparison operators:

inline bool
operator < (const Rational& lop, const Rational& rop) {
  return lop.compare(rop) < 0;
}

inline bool
operator <= (const Rational& lop, const Rational& rop) {
  return lop.compare(rop) <= 0;
}

inline bool
operator > (const Rational& lop, const Rational& rop) {
  return lop.compare(rop) > 0;
}

inline bool
operator >= (const Rational& lop, const Rational& rop) {
  return lop.compare(rop) >= 0;
}


// output to a stream

inline std::ostream&
operator<< (std::ostream& out, const Rational& n)
{
  out << n.num();
  if (n.den() != 1)
    out << "/" << n.den();
  return out;
}


// ------------------------------------------------------------------------


#endif /* !_Rational_h */

/* --- EOF Rational.h --- */
