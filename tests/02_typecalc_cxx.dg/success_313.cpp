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



/*
<testinfo>
test_generator=config/mercurium
</testinfo>
*/

template < int _n > struct C
{
    static const int value = _n;
};

template < typename T >
struct B : C<4>
{
};



template<
      typename T = void*
    , typename Tag = void*
    , typename Arity = C< B<T>::value >
    >
struct lambda;



template< template< typename P1 > class F, typename T1, typename Tag >
struct lambda< F<T1>, Tag, C< 1 > >
{
    typedef lambda< T1,Tag > l1;
};
