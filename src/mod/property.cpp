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

#include "property.h"

#include "log.h"
#include "tool/stringmanip.h"


namespace CSMOD {



Property::Property(const String& id, const String& name)
    :   idName_     (id),
        name_       (name)
{
    CSMOD_DEBUGF("Property::Property(" << id << ", " << name << ")");
}




Properties::~Properties()
{
    CSMOD_DEBUGF("Properties::~Properties() this=" << this);
    for (auto p : props_)
        delete p;
}

Property * Properties::find(const String& idName)
{
    for (auto p : props_)
        if (idName == p->idName())
            return p;
    return 0;
}


Property* Properties::add(Property * p)
{
    // check for duplicate id
    if (find(p->idName()))
    {
        String id(p->idName());
        do
        {
            increase_number(id,1);
        }
            while (find(id));

        p->idName_ = id;
    }

    props_.push_back(p);

    return p;
}

bool Properties::changed() const
{
    for (auto p : props_)
        if (p->changed()) return true;
    return false;
}


bool Properties::store(Io * io)
{
    CSMOD_DEBUGF("Properties::store(" << io << ")");

    for (auto p : props_)
    {
        io->newSection("prop");

            io->write("id", p->idName());

            if (!p->store(io))
            {
                CSMOD_IO_ERROR("could not store Property '" << p->idName() << "'");
                return false;
            }

        io->endSection();
    }

    return true;
}

bool Properties::restore(Io * io)
{
    CSMOD_DEBUGF("Properties::restore(" << io << ")");

    while (io->nextSection() && io->isSection("prop"))
    {
        String id;
        if (!io->read("id", id)) return false;
        auto prop = find(id);
        if (!prop)
        {
            CSMOD_IO_WARN("skipping unknown Property '" << id << "'");
        }
        else
        {
            CSMOD_DEBUGF("reading property '" << id << "'");

            if (!prop->restore(io))
            {
                CSMOD_IO_ERROR("could not restore Property '" << id << "'");
                return false;
            }
        }

        io->leaveSection();
    }
    return true;
}

} // namespace CSMOD
