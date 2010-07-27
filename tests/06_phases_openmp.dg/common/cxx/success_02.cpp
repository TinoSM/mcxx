/*
<testinfo>
test_generator=config/mercurium-omp
</testinfo>
*/
/*--------------------------------------------------------------------
  (C) Copyright 2006-2009 Barcelona Supercomputing Center 
                          Centro Nacional de Supercomputacion
  
  This file is part of Mercurium C/C++ source-to-source compiler.
  
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

#include <stdlib.h>

int a;

struct A
{
    int b;
    static int c;

    void f(void);

    A() : b(0) { }
};

void A::f(void)
{
#pragma omp parallel
    {
        a = 3;
        b = 4;
        c = 5;
    }
}

int main(int argc, char *argv[])
{
    A obj_a;

    a = 0;
    A::c = 0;

    obj_a.f();

    if (a != 3)
        abort();
    if (obj_a.b != 4)
        abort();
    if (A::c != 5)
        abort();

    return 0;
}
