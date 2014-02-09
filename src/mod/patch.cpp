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
#include "tool/stringmanip.h"
#include "tool/io.h"
#include "connector.h"
#include "connection.h"
#include "module.h"
#include "modulestock.h"

namespace CSMOD {


Patch::Patch()
    :   idName_ ("patch"),
        name_   ("patch")
{
    CSMOD_DEBUGF("Patch::Patch()");
}

Patch::~Patch()
{
    CSMOD_DEBUGF("Patch::~Patch()");

    for(auto m : modules_)
        delete m;
}

// ------------------ IO -------------------

bool Patch::store(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Patch::store(" << io << ")");

    if (!io->newSection("patch")) return false;
    io->write("ver", 1);

    io->write("id", idName_);
    io->write("name", name_);

    io->write("nmods", modules_.size());
    io->write("ncons", cons_.size());

    for (auto m = modules_.begin(); m != modules_.end(); ++m)
    {
        io->newSection("module");
        (*m)->store(io);
        io->endSection();
    }

    for (auto c = cons_.begin(); c != cons_.end(); ++c)
    {
        io->newSection("con");
        (*c)->store(io);
        io->endSection();
    }

    return io->endSection();
}

bool Patch::restore(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Patch::restore(" << io << ")");

    int ver;
    if (!io->read("ver", ver)) return false;
    if (ver > 1)
    {
        CSMOD_IO_ERROR("unknown patch version " << ver);
        return false;
    }

    if (!io->read("id", idName_)) return false;
    if (!io->read("name", name_)) return false;

    size_t nmods = io->readUInt("mmods"),
           ncons = io->readUInt("mcons");

    for (size_t i=0; i<nmods; ++i)
    {
        if (!io->nextSection() || !io->isSection("module"))
        {
            CSMOD_IO_ERROR("expected module");
            return false;
        }

        // create the object
        auto modclass = io->readString("class");

        auto mod = ModuleStock::instance().getModule(modclass);
        if (!addModule(mod)) return false;

        // load object properties and stuff
        mod->restore(io);

        io->endSection();
    }

    // read connections
    for (size_t i=0; i<ncons; ++i)
    {
        if (!io->nextSection() || !io->isSection("con"))
        {
            CSMOD_IO_ERROR("expected connection");
            return false;
        }

        // get module/connector id's
        std::string fm, fc, tm, tc;
        io->read("fm", fm);
        io->read("fc", fc);
        io->read("tm", tm);
        io->read("tc", tc);
        auto mod1 = findModule(fm),
             mod2 = findModule(tm);
        if (!mod1) { CSMOD_IO_ERROR("unknown module id '" << fm << "' in connection"); continue; }
        if (!mod2) { CSMOD_IO_ERROR("unknown module id '" << tm << "' in connection"); continue; }
        auto c1 = mod1->findConnector(fc),
             c2 = mod2->findConnector(tc);
        if (!c1) { CSMOD_IO_ERROR("unknown connnector id '" << fm << "." << fc << "' in connection"); continue; }
        if (!c2) { CSMOD_IO_ERROR("unknown connnector id '" << tm << "." << tc << "' in connection"); continue; }
        // make the connection
        auto con = connect(c1, c2);
        // read additional data
        con->restore(io);

        if (!io->endSection()) return false;
    }

    return true;
}


// -------------------- modules ------------------------

bool Patch::hasModule(const Module * mod)
{
    CSMOD_DEBUGF("Patch::hasModule(" << mod << ")");

    for (auto m : modules_)
        if (mod == m)
            return true;
    return false;
}

Module * Patch::findModule(const std::string& idname)
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

    // check presense
    if (hasModule(module))
    {
        CSMOD_RT_ERROR("attempt to add module '" << module->name() << "' again.");
        return false;
    }

    // adjust idName
    while (findModule(module->idName_))
        increase_number(module->idName_, 1);

    modules_.push_back(module);

    // assign patch to module
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
