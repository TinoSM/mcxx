/*
<testinfo>
test_generator=config/mercurium-omp
test_compile_fail_nanox_plain=yes
test_compile_faulty_nanox_plain=yes
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

int a[10];

int main(int argc, char *argv[])
{
    int b[10];
    int i;

    a[1] = 3;
    a[2] = 2;
    b[4] = 4;
    b[5] = 5;

#pragma omp for firstprivate(a, b) lastprivate(a, b)
    for (i = 0; i < 10; i++)
    {
        a[1] = a[2];
        b[4] = b[5];
    }

    if (a[1] != 23) 
        abort();
    if (a[2] != 2) 
        abort();
    if (b[4] != 54) 
        abort();
    if (b[5] != 5) 
        abort();

    return 0;
}
