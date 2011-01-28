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


#include "tl-devices.hpp"

using namespace TL;
using namespace TL::Nanox;

static DeviceHandler* _nanox_handler = 0;

DeviceHandler& DeviceHandler::get_device_handler()
{
    if (_nanox_handler == 0)
    {
        _nanox_handler = new DeviceHandler();
    }
    return *_nanox_handler;
}

void DeviceHandler::register_device(const std::string& str, DeviceProvider* nanox_device_provider)
{
    _nanox_devices[str] = nanox_device_provider;
}

DeviceProvider::DeviceProvider(const std::string& device_name, bool needs_copies)
        : _device_name(device_name),
        _enable_instrumentation(false), 
        _enable_instrumentation_str(""),
        _needs_copies(needs_copies)
{
    DeviceHandler &device_handler(DeviceHandler::get_device_handler());               
    device_handler.register_device(device_name, this);
    
    register_parameter("instrument", 
                       "Enables instrumentation of the device provider if set to '1'",
                       _enable_instrumentation_str,
                       "0").connect(functor(&DeviceProvider::set_instrumentation, *this));
}

DeviceProvider* DeviceHandler::get_device(const std::string& str)
{
    nanox_devices_map_t::iterator it = _nanox_devices.find(str);

    if (it == _nanox_devices.end())
        return NULL;
    else
        return it->second;
}
