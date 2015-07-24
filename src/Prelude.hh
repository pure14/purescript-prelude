///////////////////////////////////////////////////////////////////////////////
//
// Module      :  Prelude.hh
// Copyright   :  (c) Andy Arvanitis 2015
// License     :  MIT
//
// Maintainer  :  Andy Arvanitis <andy.arvanitis@gmail.com>
// Stability   :  experimental
// Portability :
//
// Prelude FFI functions
//
///////////////////////////////////////////////////////////////////////////////
//
#ifndef Prelude_FFI_HH
#define Prelude_FFI_HH

#include "Purescript/Purescript.hh"

namespace Prelude {
  using namespace PureScript;

  //- Functor --------------------------------------------------------------------

  // TODO: Inefficient
  template <typename A, typename B>
  inline auto arrayMap(fn<A,B> f) -> fn<array<A>,array<B>> {
    return [=](array<A> a) {
      array<B> b;
      for (auto it = a.begin(); it != a.end(); ++it) {
          b = array<B>(b,f(*it));
      }
      return b;
    };
  }

  //- Bind -----------------------------------------------------------------------

  // TODO: Inefficient?
  template <typename A, typename B>
  inline auto arrayBind(array<A> a) -> fn<fn<A,array<B>>,array<B>> {
    return [=](fn<A,array<B>> f) {
      array<B> b;
      for (auto it = a.begin(); it != a.end(); ++it) {
          b = array<B>(b,f(*it));
      }
      return b;
    };
  }

  //- Monoid ---------------------------------------------------------------------

  inline auto concatString(string x) -> fn<string,string> {
    return [=](string y) {
      return x + y;
    };
  }

  template <typename A>
  inline auto concatArray(array<A> a) -> fn<array<A>,array<A>> {
    return [=](array<A> b) {
      return a.append(b);
    };
  }

  //- Semiring -------------------------------------------------------------------

  inline auto intAdd(long x) -> fn<long,long> {
    return [=](long y) {
      return x + y;
    };
  }

  inline auto intMul(long x) -> fn<long,long> {
    return [=](long y) {
      return x * y;
    };
  }

  inline auto numAdd(double x) -> fn<double,double> {
    return [=](double y) {
      return x + y;
    };
  }

  inline auto numMul(double x) -> fn<double,double> {
    return [=](double y) {
      return x * y;
    };
  }

  //- ModuloSemiring -------------------------------------------------------------

  inline auto intDiv(long x) -> fn<long,long> {
    return [=](long y) {
      return x / y;
    };
  }

  inline auto intMod(long x) -> fn<long,long> {
    return [=](long y) {
      return x % y;
    };
  }

  inline auto numDiv(double x) -> fn<double,double> {
    return [=](double y) {
      return x / y;
    };
  }

  //- Ring -----------------------------------------------------------------------

  inline auto intSub(long x) -> fn<long,long> {
    return [=](long y) {
      return x - y;
    };
  }

  inline auto numSub(double x) -> fn<double,double> {
    return [=](double y) {
      return x - y;
    };
  }

  //- Eq -------------------------------------------------------------------------

  template <typename T>
  inline auto refEq(T ref1) -> fn<T,bool> {
    return [=](T ref2) {
      return ref1 == ref2;
    };
  }

  template <typename T>
  inline auto refIneq(T ref1) -> fn<T,bool> {
    return [=](T ref2) {
      return ref1 != ref2;
    };
  }

  template <typename A>
  inline auto eqArrayImpl(fn<A,fn<A,bool>> f) -> fn<array<A>,fn<array<A>,bool>> {
    return [=](array<A> xs) {
      return [=](array<A> ys) {
        return xs == ys;
      };
    };
  }

  template <typename A>
  inline auto ordArrayImpl(fn<A,fn<A,int>> f) -> fn<array<A>,fn<array<A>,int>> {
    return [=](array<A> xs) {
      return [=](array<A> ys) {
        typename decltype(xs)::size_type i = 0;
        const auto xlen = xs.size();
        const auto ylen = ys.size();
        while (i < xlen && i < ylen) {
          const auto x = xs[i];
          const auto y = ys[i];
          const auto o = f(x)(y);
          if (o != 0) {
            return o;
          }
          i++;
        }
        if (xlen == ylen) {
          return 0;
        } else if (xlen > ylen) {
          return -1;
        } else {
          return 1;
        }
      };
    };
  }

  //- Ord ------------------------------------------------------------------------

  namespace type {
    struct Ordering;
  }

  template <typename T>
  inline auto unsafeCompareImpl(managed<type::Ordering> lt) ->
      fn<managed<type::Ordering>,fn<managed<type::Ordering>,fn<T,fn<T,managed<type::Ordering>>>>> {
    return [=](managed<type::Ordering> eq) -> fn<managed<type::Ordering>,fn<T,fn<T,managed<type::Ordering>>>> {
      return [=](managed<type::Ordering> gt) -> fn<T,fn<T,managed<type::Ordering>>> {
        return [=](T x) -> fn<T,managed<type::Ordering>> {
          return [=](T y) -> managed<type::Ordering> {
            return x < y ? lt : x > y ? gt : eq;
          };
        };
      };
    };
  }

  //- Lattice --------------------------------------------------------------------

  inline auto boolOr(bool x) -> fn<bool,bool> {
    return [=](bool y) {
      return x || y;
    };
  }

  inline auto boolAnd(bool x) -> fn<bool,bool> {
    return [=](bool y) {
      return x && y;
    };
  }

  //- ComplementedLattice --------------------------------------------------------

  inline auto boolNot(bool x) -> bool {
    return !x;
  }

  //- Show -----------------------------------------------------------------------

  inline auto showIntImpl(long x) -> string {
    return std::to_string(x);
  }

  inline auto showNumberImpl(double x) -> string {
    return std::to_string(x);
  }

  inline auto showCharImpl(char c) -> string {
    string s("'");
    s.push_back(c);
    s.push_back('\'');
    return s;
  }

  inline auto showStringImpl(string s) -> string {
    return '"' + s + '"';
  }

  template <typename A>
  inline auto showArrayImpl(fn<A,string> f) -> fn<array<A>,string> {
    return [=](array<A> xs) -> string {
      string s("[");
      auto count = xs.size();
      for (auto it = xs.begin(); it != xs.end(); ++it) {
        s.append(f(*it));
        if (--count > 0) {
          s.push_back(',');
        }
      }
      s.push_back(']');
      return s;
    };
  }

}

#endif // Prelude_FFI_HH
