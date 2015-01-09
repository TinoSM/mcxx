/*--------------------------------------------------------------------
  (C) Copyright 2006-2012 Barcelona Supercomputing Center
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
  See AUTHORS file in the top level directory for information
  regarding developers and contributors.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 3 of the License, or (at your option) any later version.
  
  Mercurium C/C++ source-to-source compiler is distributed in the hope
  that it will be useful, but WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the GNU Lesser General Public License for more
  details.
  
  You should have received a copy of the GNU Lesser General Public
  License along with Mercurium C/C++ source-to-source compiler; if
  not, write to the Free Software Foundation, Inc., 675 Mass Ave,
  Cambridge, MA 02139, USA.
--------------------------------------------------------------------*/


#ifndef TL_FUNCTOR_HPP
#define TL_FUNCTOR_HPP

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tl-common.hpp"
#include <functional>
#include "cxx-macros.h"

#if !defined(HAVE_CXX11)
#include <tr1/functional>
namespace std
{
    using std::tr1::function;
    using std::tr1::bind;
    // Note: std::tr1::result_of behaves slightly different to std::result_of
    // so we prefer to make sure we acknowledge the difference
    // Note: std::tr1::result_of requires a result_type if a function object
    // is created using a class

    namespace placeholders {
        using std::tr1::placeholders::_1;
        // using std::tr1::placeholders::_2;
    }
}
#endif


template <typename T, typename F>
struct LiftPointer
{
    private:
#if defined(HAVE_CXX11)
        typedef typename std::result_of<F(T)>::type Ret;
#else
        typedef typename std::tr1::result_of<F(T)>::type Ret;
#endif
        std::function<Ret(T)> _f;

    public:
    LiftPointer(F f)
        : _f(f) { }

    Ret operator ()(T *t) const
    {
        return _f(*t);
    }

#if !defined(HAVE_CXX11)
    typedef Ret result_type;
#endif
};


template <typename T, typename F>
LiftPointer<T, F> lift_pointer(F fun)
{
    LiftPointer<T, F> lifted(fun);
    return lifted;
}


#endif // TL_FUNCTOR_HPP
