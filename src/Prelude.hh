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

#include "PureScript/PureScript.hh"

namespace Prelude {
  using namespace PureScript;

  //- Functor --------------------------------------------------------------------

  inline auto arrayMap(const any& f) -> any {
    return [=](const any& a) -> any {
      const any::vector& as = a;
      any::vector bs;
      for (auto it = as.begin(); it != as.end(); ++it) {
          bs.push_back(f(*it));
      }
      return bs;
    };
  }

  //- Bind -----------------------------------------------------------------------

  inline auto arrayBind(const any& a) -> any {
    return [=](const any& f) -> any {
      const any::vector& as = a;
      any::vector bs;
      for (auto it = as.begin(); it != as.end(); ++it) {
        any::vector xs = f(*it);
        bs.insert(bs.end(), xs.begin(), xs.end());
      }
      return bs;
    };
  }

  //- Monoid ---------------------------------------------------------------------

  inline auto concatString(const any& s1) -> any {
    return [=](const any& s2) -> any {
      return s1 + s2;
    };
  }

  inline auto concatArray(const any& a) -> any {
    return [=](const any& b) -> any {
      any::vector xs = a; // makes a copy
      const any::vector& ys = b;
      xs.insert(xs.end(), ys.begin(), ys.end());
      return xs;
    };
  }

  //- Semiring -------------------------------------------------------------------

  inline auto intAdd(const any& x) -> any {
    return [=](const any& y) -> any {
      return x + y;
    };
  }

  inline auto intMul(const any& x) -> any {
    return [=](const any& y) -> any {
      return x * y;
    };
  }

  inline auto numAdd(const any& x) -> any {
    return [=](const any& y) -> any {
      return x + y;
    };
  }

  inline auto numMul(const any& x) -> any {
    return [=](const any& y) -> any {
      return x * y;
    };
  }

  //- ModuloSemiring -------------------------------------------------------------

  inline auto intDiv(const any& x) -> any {
    return [=](const any& y) -> any {
      return x / y;
    };
  }

  inline auto intMod(const any& x) -> any {
    return [=](const any& y) -> any {
      return x % y;
    };
  }

  inline auto numDiv(const any& x) -> any {
    return [=](const any& y) -> any {
      return x / y;
    };
  }

  //- Ring -----------------------------------------------------------------------

  inline auto intSub(const any& x) -> any {
    return [=](const any& y) -> any {
      return x - y;
    };
  }

  inline auto numSub(const any& x) -> any {
    return [=](const any& y) -> any {
      return x - y;
    };
  }

  //- Eq -------------------------------------------------------------------------

  inline auto refEq(const any& ref1) -> any {
    return [=](const any& ref2) -> any {
      return ref1 == ref2;
    };
  }

  inline auto refIneq(const any& ref1) -> any {
    return [=](const any& ref2) -> any {
      return ref1 != ref2;
    };
  }

  inline auto eqArrayImpl(const any& f) -> any {
    return [=](const any& xs_) -> any {
      return [=](const any& ys_) -> any {
        const any::vector& xs = xs_;
        const any::vector& ys = ys_;
        const auto xs_size = xs.size();
        if (xs_size != ys.size()) {
          return false;
        }
        for (any::vector::size_type i = 0; i < xs_size; i++) {
          const auto res = f(xs[i])(ys[i]);
          if (!res.cast<bool>()) {
            return false;
          }
        }
        return true;
      };
    };
  }

  inline auto ordArrayImpl(const any& f) -> any {
    return [=](const any& xs_) -> any {
      return [=](const any& ys_) -> any {
        const any::vector& xs = xs_;
        const any::vector& ys = ys_;
        any::vector::size_type i = 0;
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

  inline auto unsafeCompareImpl(const any& lt) -> any {
    return [=](const any& eq) -> any {
      return [=](const any& gt) -> any {
        return [=](const any& x) -> any {
          return [=](const any& y) -> any {
            return x < y ? lt : x > y ? gt : eq;
          };
        };
      };
    };
  }

  //- Lattice --------------------------------------------------------------------

  inline auto boolOr(const any& x) -> any {
    return [=](const any& y) -> any {
      return x.cast<bool>() || y.cast<bool>();
    };
  }

  inline auto boolAnd(const any& x) -> any {
    return [=](const any& y) -> any {
      return x.cast<bool>() && y.cast<bool>();
    };
  }

  //- ComplementedLattice --------------------------------------------------------

  inline auto boolNot(const any& x) -> any {
    return !x.cast<bool>();
  }

  //- Show -----------------------------------------------------------------------

  inline auto showIntImpl(const any& x) -> any {
    return std::to_string(x.cast<long>());
  }

  inline auto showNumberImpl(const any& x) -> any {
    return std::to_string(x.cast<double>());
  }

  inline auto showCharImpl(const any& c) -> any {
    string s("'");
    s.push_back(c.cast<char>());
    s.push_back('\'');
    return s;
  }

  inline auto showStringImpl(const any& s) -> any {
    return '"' + s.cast<string>() + '"';
  }

  inline auto showArrayImpl(const any& f) -> any {
    return [=](const any& xs_) -> any {
      const any::vector& xs = xs_;
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
