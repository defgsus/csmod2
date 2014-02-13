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

// this will be connected to the DspGraph and GUI class later
#define CSMOD_GRAPH_ERROR(arg__) { std::cerr << "DSP GRAPH ERROR: " << arg__ << std::endl; }

namespace CSMOD {

// ------------------ internal structs --------------------

// input and output connections
struct DspGraph::ModuleEdge
{
    ModuleEdge(ModuleNode * mod)
        :   mod(mod), present(true)
    { }

    // parent node
    ModuleNode * mod;

    // flag to remove edges without acually changing the container
    bool present;

};

// represent a module + sum locally needed helper
struct DspGraph::ModuleNode
{
    ModuleNode(DspModule * m)
        : module(m)
    { }

    // represented module
    DspModule * module;
    // ingoing and outgoing connections
    std::vector<ModuleEdge>
        ins, outs;

    // returns number of input nodes (checks ModuleEdge::present)
    size_t numInputs()
    {
        size_t count = 0;
        for (auto &i : ins)
            if (i.present) ++count;
        return count;
    }

    void addInputNonDuplicate(ModuleNode * m)
    {
        for (auto &i : ins)
            if (i.mod->module == m->module)
                return;
        std::cout << "ADDED IN " << module->idName() << " -> " << m->module->idName() << "\n";
        ins.push_back(ModuleEdge(m));
    }

    void addOutputNonDuplicate(ModuleNode * m)
    {
        for (auto &i : outs)
            if (i.mod->module == m->module)
                return;
        std::cout << "ADDED OUT " << module->idName() << " -> " << m->module->idName() << "\n";
        outs.push_back(ModuleEdge(m));
    }
};


// ----------------------- ctor -----------------------

DspGraph::DspGraph()
    :   patch_  (0)
{
    CSMOD_DEBUGF("DspGraph::DspGraph()");
}

DspGraph::~DspGraph()
{
    CSMOD_DEBUGF("DspGraph::~DspGraph()");
    clear();
}


// ----------------- calculation --------------------

void DspGraph::clear()
{
    CSMOD_DEBUGF("DspGraph::clear()");

    patch_ = 0;
    for (auto m = map_.begin(); m != map_.end(); ++m)
        delete m->second;
    map_.clear();
    modules_.clear();
}

bool DspGraph::initFromPatch(Patch * patch)
{
    CSMOD_DEBUGF("DspGraph::initFromPatch(" << patch << ")");

    clear();

    patch_ = patch;

    // get structure of patch
    initMap_();

#ifndef NDEBUG
    dump_();
#endif

    return traceGraph_();
}

void DspGraph::getSortedModules(DspModules& modules)
{
    CSMOD_DEBUGF("DspGraph::getSortedModules(...)");
    modules = modules_;
}



// ------------------- implementation ------------------

void DspGraph::initMap_()
{
    CSMOD_DEBUGF("DspGraph::initMap_()");

    // add all dsp modules to map_
    for (auto m : patch_->modules())
    if (auto dsp = dynamic_cast<DspModule*>(m))
    {
        // create a node for each module
        auto node = new ModuleNode(dsp);
        map_.insert(std::make_pair(dsp, node));
    }

    // add connections
    // NOTE: we could use the info that is already
    // in Connector::modules() but below method is
    // more convenient to code
    for (auto c : patch_->connections())
    {
        auto m1 = dynamic_cast<DspModule*>(c->moduleFrom()),
             m2 = dynamic_cast<DspModule*>(c->moduleTo());
        auto i1 = map_.find(m1),
             i2 = map_.find(m2);
        if (i1 != map_.end() && i2 != map_.end())
        {
            i1->second->addOutputNonDuplicate(i2->second);
            i2->second->addInputNonDuplicate(i1->second);
            //i1->second->outs.push_back(ModuleEdge(i2->second));
            //i2->second->ins.push_back(ModuleEdge(i1->second));
        }
    }
}

/* http://en.wikipedia.org/wiki/Topological_sorting */
bool DspGraph::traceGraph_()
{
    CSMOD_DEBUGF("DspGraph::traceGraph_()");

    // nodes with no inputs (temporary space)
    std::vector<ModuleNode*> mods;

    // modules with only outputs
    // are at the start of calculation
    for (auto m = map_.begin(); m != map_.end(); ++m)
    {
        if (m->second->ins.empty() && m->second->outs.size())
            mods.push_back(m->second);
    }

    if (mods.empty())
    {
        CSMOD_GRAPH_ERROR("no input modules in patch");
        return false;
    }

    while (!mods.empty())
    {
        // remove a node from mods (input-only-modules)
        auto m = mods.back();
        mods.pop_back();
        // move to sorted list
        modules_.push_back(m->module);

        // for each outgoing edge
        for (auto &o : m->outs)
        {
            // remove this edge
            o.present = false;
            for (auto &e : o.mod->ins)
                if (e.mod == m) { e.present = false; break; }

            // if no other input node, move to mods
            if (o.mod->numInputs() == 0)
            {
                mods.push_back(o.mod);
            }
        }
    }

    // check if edges left
    for (auto &m : map_)
    {
        if (m.second->numInputs())
        {
            CSMOD_GRAPH_ERROR("detected loop in patch at module '" << m.first->idName() << "'");
            return false;
        }
    }

    std::cout << "--- sorted ---\n";
    for (auto &m : modules_)
    {
        std::cout << m->idName() << "\n";
    }

    return true;
}

void DspGraph::dump_()
{
    CSMOD_DEBUGF("DspGraph::dump_()");

    for (auto m = map_.begin(); m != map_.end(); ++m)
    {
        std::cout << "\n-- " << m->first->idName() << ":\n"
                  << "in :";
        for (auto &i : m->second->ins)
            std::cout << " " << i.mod->module->idName();
        std::cout << "\nout:";
        for (auto &i : m->second->outs)
            std::cout << " " << i.mod->module->idName();
        std::cout << "\n\n";
        std::cout.flush();
    }
}


} // namespace CSMOD
