///////////////////////////////////////////////////////////////////////////////
//
// Module      :  Prelude.cc
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
#include "Prelude.hh"

namespace Prelude {
  using namespace PureScript;

  //- Functor --------------------------------------------------------------------

  auto arrayMap(const any& f) -> any {
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

  auto arrayBind(const any& a) -> any {
    return [=](const any& f) -> any {
      const auto& as = a.cast<any::vector>();
      any::vector bs;
      for (auto it = as.begin(); it != as.end(); ++it) {
        auto xs_ = f(*it);
        const auto& xs = xs_.cast<any::vector>();
        bs.insert(bs.end(), xs.begin(), xs.end());
      }
      return bs;
    };
  }

  //- Monoid ---------------------------------------------------------------------

  auto concatArray(const any& a) -> any {
    return [=](const any& b) -> any {
      any::vector xs(a.cast<any::vector>()); // makes a copy
      const auto& ys = b.cast<any::vector>();
      xs.insert(xs.end(), ys.begin(), ys.end());
      return xs;
    };
  }

  //- Eq -------------------------------------------------------------------------

  auto eqArrayImpl(const any& f) -> any {
    return [=](const any& xs_) -> any {
      return [=](const any& ys_) -> any {
        const auto& xs = xs_.cast<any::vector>();
        const auto& ys = ys_.cast<any::vector>();
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

  auto ordArrayImpl(const any& f) -> any {
    return [=](const any& xs_) -> any {
      return [=](const any& ys_) -> any {
        const auto& xs = xs_.cast<any::vector>();
        const auto& ys = ys_.cast<any::vector>();
        any::vector::size_type i = 0;
        const auto xlen = xs.size();
        const auto ylen = ys.size();
        while (i < xlen && i < ylen) {
          const auto x = xs[i];
          const auto y = ys[i];
          const auto o = f(x)(y);
          if (o != 0L) {
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

  auto unsafeCompareImpl(const any& lt) -> any {
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

  //- Show -----------------------------------------------------------------------

  auto showCharImpl(const any& c) -> any {
    string s("'");
    s.push_back(c.cast<char>());
    s.push_back('\'');
    return s;
  }

  auto showStringImpl(const any& s) -> any {
    return '"' + s.cast<string>() + '"';
  }

  auto showArrayImpl(const any& f) -> any {
    return [=](const any& xs_) -> any {
      const auto& xs = xs_.cast<any::vector>();
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
