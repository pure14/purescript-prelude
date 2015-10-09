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

  auto arrayMap(const any&) -> any;

  //- Bind -----------------------------------------------------------------------

  auto arrayBind(const any&) -> any;

  //- Monoid ---------------------------------------------------------------------

  inline auto concatString(const any& s1) -> any {
    return [=](const any& s2) -> any {
      return s1 + s2;
    };
  }

  auto concatArray(const any&) -> any;

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

  auto eqArrayImpl(const any&) -> any;

  auto ordArrayImpl(const any&) -> any;

  //- Ord ------------------------------------------------------------------------

  auto unsafeCompareImpl(const any&) -> any;

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

  auto showCharImpl(const any&) -> any;

  auto showStringImpl(const any& s) -> any;

  auto showArrayImpl(const any& f) -> any;

}

#endif // Prelude_FFI_HH
