/*--------------------------------------------------------------------
  (C) Copyright 2006-2014 Barcelona Supercomputing Center
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


#include "tl-lowering-visitor.hpp"
#include "cxx-diagnostic.h"

namespace TL { namespace Intel {

LoweringVisitor::LoweringVisitor(Lowering* lowering)
    : _lowering(lowering)
{
}

LoweringVisitor::~LoweringVisitor()
{
}

void LoweringVisitor::visit(const Nodecl::OpenMP::Atomic& construct)
{
    error_printf("%s: error: OpenMP Atomic construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::Critical& construct)
{
    error_printf("%s: error: OpenMP Critical construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::FlushMemory& construct)
{
    error_printf("%s: error: OpenMP FlushMemory construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::Sections& construct)
{
    error_printf("%s: error: OpenMP Sections construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::Workshare& construct)
{
    error_printf("%s: error: OpenMP Workshare construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::TargetDeclaration& construct)
{
    error_printf("%s: error: OpenMP TargetDeclaration construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::Task& construct)
{
    error_printf("%s: error: OpenMP Task construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::TaskCall& construct)
{
    error_printf("%s: error: OpenMP TaskCall construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::TaskExpression& construct)
{
    error_printf("%s: error: OpenMP TaskExpression construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::TaskwaitShallow& construct)
{
    error_printf("%s: error: OpenMP TaskwaitShallow construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

void LoweringVisitor::visit(const Nodecl::OpenMP::WaitOnDependences& construct)
{
    error_printf("%s: error: OpenMP WaitOnDependences construct not yet implemented\n", 
            construct.get_locus_str().c_str());
}

} }
