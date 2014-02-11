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

#include "dspgraph.h"

#include "log.h"
#include "module.h"
#include "dspmodule.h"
#include "patch.h"

namespace CSMOD {

DspGraph::DspGraph()
    :   patch_  (0)
{
    CSMOD_DEBUGF("DspGraph::DspGraph()");
}

DspGraph::~DspGraph()
{
    CSMOD_DEBUGF("DspGraph::~DspGraph()");
}

// -------------- init ---------------

void DspGraph::clear()
{
    CSMOD_DEBUGF("DspGraph::clear()");

    patch_ = 0;
    for (auto m = map_.begin(); m != map_.end(); ++m)
        delete m->second;
    map_.clear();
}

bool DspGraph::initFromPatch(Patch * patch)
{
    CSMOD_DEBUGF("DspGraph::initFromPatch(" << patch << ")");

    clear();

    patch_ = patch;

    initMap_();

    dump_();

    return true;
}

void DspGraph::initMap_()
{
    // first add all modules to map_
    for (auto m : patch_->modules())
    if (dynamic_cast<DspModule*>(m))
    {
        auto mod = new Mod;
        mod->module = m;
        mod->visited = false;

        map_.insert(std::make_pair(m, mod));
    }

    // add connections
    for (auto c : patch_->connections())
    {
        auto i1 = map_.find(c->moduleFrom()),
             i2 = map_.find(c->moduleTo());
        if (i1 != map_.end() && i2 != map_.end())
        {
            i1->second->outs.push_back(i2->second);
            i2->second->ins.push_back(i1->second);
        }
    }
}

void DspGraph::dump_()
{
    for (auto m = map_.begin(); m != map_.end(); ++m)
    {
        std::cout << "\n-- " << m->first->idName() << ":\n"
                  << "in :";
        for (auto i : m->second->ins)
            std::cout << " " << i->module->idName();
        std::cout << "\nout:";
        for (auto i : m->second->outs)
            std::cout << " " << i->module->idName();
        std::cout << "\n\n";
        std::cout.flush();
    }
}

// ------------ runtime --------------

void DspGraph::dspStep()
{

}

} // namespace CSMOD
