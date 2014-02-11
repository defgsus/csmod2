/***************************************************************************

Copyright (C) 2014  stefan.berke @ modular-audio-graphics.com

This source is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

****************************************************************************/

#include "connector.h"

#include "log.h"
#include "module.h"


namespace CSMOD {

// -------- connections -------------

bool Connector::connectModule(Module * module)
{
    if (isConnectedTo(module))
    {
        CSMOD_RT_WARN("duplicate request of connection " << module_->idName()
                      << " -> " << module->idName());
        return false;
    }

    modules_.push_back(module);
    return true;
}

bool Connector::disconnectModule(Module * module)
{
    for (auto m = modules_.begin(); m!=modules_.end(); ++m)
    {
        if (*m == module)
        {
            modules_.erase(m);
            return true;
        }
    }
    return false;
}

bool Connector::isConnectedTo(Module * module)
{
    for (auto &m : modules_)
        if (m == module) return true;
    return false;
}

} // namespace CSMOD
