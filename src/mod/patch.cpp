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

#include "patch.h"

#include "log.h"
#include "connector.h"
#include "connection.h"
#include "module.h"

namespace CSMOD {


Patch::Patch()
{
    CSMOD_DEBUGF("Patch::Patch()");
}

Patch::~Patch()
{
    CSMOD_DEBUGF("Patch::~Patch()");

    for(auto m : modules_)
        delete m;
}


// -------------------- modules ------------------------

Module * Patch::getModule(const std::string& idname)
{
    CSMOD_DEBUGF("Patch::getModule(" << idname << ")");

    for (auto m : modules_)
        if (idname == m->idName())
            return m;
    return 0;
}

bool Patch::addModule(Module * module)
{
    CSMOD_DEBUGF("Patch::addModule(" << module << ")");

    modules_.push_back(module);
    module->patch_ = this;
    return true;
}



// --------------- connections -------------------------

Connection * Patch::connect(Connector * con1, Connector * con2)
{
    CSMOD_DEBUGF("Patch::connect(" << con1 << ", " << con2 << ")");

    if (!con1 || !con2) return 0;
    if (con1->module()->patch() != this
        || con2->module()->patch() != this)
    {
        CSMOD_RT_ERROR("Can not connect");
        return 0;
    }

    auto c = new Connection(con1, con2);

    cons_.push_back(c);

    return c;
}

bool Patch::disconnect(Connection * con)
{
    CSMOD_DEBUGF("Patch::disconnect(" << con << ")");

    for (auto i = cons_.begin(); i!=cons_.end(); ++i)
    if (*i == con)
    {
        cons_.erase(i);
        delete *i;
        return true;
    }

    return false;
}

} // namespace CSMOD
