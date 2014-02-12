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

#include "module.h"

#include "log.h"
#include "tool/io.h"
#include "tool/stringmanip.h"

namespace CSMOD {

Module::Module(const std::string& idName, const std::string& className)
    :   patch_    (0),
        className_(className),
        idName_   (idName),
        name_     (className)
{
    CSMOD_DEBUGF("Module::Module(\"" << idName << "\", \"" << className << "\")");

    // make sure the strings are right
    checkIdName(className_);
    checkIdName(idName_);
}

Module::~Module()
{
    CSMOD_DEBUGF("Module::~Module()");

    deleteConnectors_();
}


// ------------------ IO -------------------

#define CSMOD_CHECKIO(command__, errortext__) \
    if (!(command__)) { CSMOD_IO_ERROR(errortext__); return false; }

bool Module::store(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Module::store(" << io << ")");

    CSMOD_CHECKIO(io->write("ver", 1), "could not write module version");
    io->write("class", className_);
    io->write("id", idName_);
    io->write("name", name_);

    return true;
}

bool Module::restore(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Module::restore(" << io << ")");

    int ver;
    CSMOD_CHECKIO(io->read("ver", ver), "could not read module version");
    CSMOD_CHECKIO(ver <= 1, "unknown module version " << ver);

    CSMOD_CHECKIO(io->read("id", idName_), "could not read module id");
    CSMOD_CHECKIO(io->read("name", name_), "could not read module name");

    return true;
}

#undef CSMOD_CHECKIO

// ------------------- connectors ---------------------

Connector * Module::findConnector(const std::string& idName)
{
    for (auto c : cons_)
        if (idName == c->idName())
            return c;
    return 0;
}

Connector* Module::add_(Connector * c)
{
    // check for duplicate id
    if (findConnector(c->idName()))
    {
        std::string id(c->idName());
        do
        {   increase_number(id,1);

        } while (findConnector(id));

        c->idName_ = id;
    }

    cons_.push_back(c);

    return c;
}

void Module::deleteConnectors_()
{
    for (auto c : cons_)
        delete c;
    cons_.clear();
}


// ------------- debug ---------------

void Module::debug_dump()
{
    std::cout << "module " << this << " " << idName() << "\n";
    for (auto i : cons_)
    {
        i->debug_dump();
    }
    std::cout << "\n";
}


} // namespace CSMOD
