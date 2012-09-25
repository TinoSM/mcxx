/*--------------------------------------------------------------------
  (C) Copyright 2006-2011 Barcelona Supercomputing Center 
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



#ifndef TL_OMP_TARGET_HPP
#define TL_OMP_TARGET_HPP

#include "tl-objectlist.hpp"
#include "tl-symbol.hpp"

namespace TL
{
    namespace OpenMP
    {
        // This struct controls we do not add the same device implementation
        // twice, uses device_name + ctr.get_locus() as id
        static ObjectList<std::string> added_devices_pos;

        struct TargetContext
        {
            ObjectList<std::string> device_list;

            ObjectList<std::string> copy_in;
            ObjectList<std::string> copy_out;
            ObjectList<std::string> copy_inout;
            std::string ndrange;
            std::string calls;

            bool has_implements;
            Symbol implements;

            bool copy_deps;

            TargetContext()
                : device_list(), 
                copy_in(), 
                copy_out(), 
                has_implements(), 
                implements(), 
                copy_deps(),
                ndrange(),
                calls()
            {
            }
        };
    }
}

#endif // TL_OMP_TARGET_HPP
