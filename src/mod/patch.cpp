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
#include "dspgraph.h"
#include "module/dsp/audioout.h"

namespace CSMOD {


Patch::Patch()
    :   idName_         ("patch"),
        name_           ("patch"),
        audioInModule_  (0),
        audioOutModule_ (0),
        blockSize_      (0),
        numChannelsIn_  (0),
        numChannelsOut_ (0)
{
    CSMOD_DEBUGF("Patch::Patch()");
}

Patch::~Patch()
{
    CSMOD_DEBUGF("Patch::~Patch()");

    for (auto c : cons_)
        delete c;
    for (auto m : modules_)
        delete m;
}


#define CSMOD_CHECKIO(command__, errortext__) \
    if (!(command__)) { CSMOD_IO_ERROR(errortext__); return false; }

// ------------------ IO -------------------

bool Patch::store(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Patch::store(" << io << ")");

    if (!io->newSection("patch")) return false;
    io->write("ver", 1);

    io->write("id", idName_);
    io->write("name", name_);

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
    CSMOD_CHECKIO(io->read("ver", ver), "could not read patch version");
    CSMOD_CHECKIO(ver <= 1, "unknown patch version " << ver);

    CSMOD_CHECKIO(io->read("id", idName_), "could not read id");
    CSMOD_CHECKIO(io->read("name", name_), "could not read name");

    // read all modules and connections
    while (io->nextSection())
    {
        // a module
        if (io->isSection("module"))
        {

            // --- create the object ---

            auto modclass = io->readString("class");

            auto mod = ModuleStock::instance().getModule(modclass);
            if (!mod)
            {
                CSMOD_IO_WARN("skipping unknown module class " << modclass);
                goto skip_;
            }
            CSMOD_CHECKIO(addModule(mod), "could not create module " << modclass);

            // load object properties and stuff
            CSMOD_CHECKIO(mod->restore(io), "could not load module " << modclass);

            mod->applyProperties();
        }
        else
        // a connection
        if (io->isSection("con"))
        {
            // get module/connector id's
            String fm, fc, tm, tc;
            io->read("fm", fm);
            io->read("fc", fc);
            io->read("tm", tm);
            io->read("tc", tc);
            auto mod1 = findModule(fm),
                 mod2 = findModule(tm);
            if (!mod1) { CSMOD_IO_WARN("unknown module id '" << fm << "' in connection"); goto skip_; }
            if (!mod2) { CSMOD_IO_WARN("unknown module id '" << tm << "' in connection"); goto skip_; }
            auto c1 = mod1->findConnector(fc),
                 c2 = mod2->findConnector(tc);
            if (!c1) { CSMOD_IO_WARN("unknown connnector id '" << fm << "." << fc << "' in connection"); goto skip_; }
            if (!c2) { CSMOD_IO_WARN("unknown connnector id '" << tm << "." << tc << "' in connection"); goto skip_; }
            // make the connection
            auto con = connect(c1, c2);
            // read additional data
            CSMOD_CHECKIO(con->restore(io), "could not restore connection");
        }

    skip_:
        io->leaveSection();
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

Module * Patch::findModule(const String& idname)
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

    // check if present
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

    // copy patch settings
    module->setSampleRate(sampleRate_);

    // initialize module
    module->applyProperties();

    updateDspGraph();

    return true;
}



// --------------- connections -------------------------

bool Patch::isConnected(Connector * con1, Connector * con2) const
{
    CSMOD_DEBUGF("Patch::isConnected(" << con1 << ", " << con2 << ")");
    return (con1->isConnectedTo(con2) ||
            con2->isConnectedTo(con1));
}

Connection * Patch::connect(Connector * con1, Connector * con2)
{
    CSMOD_DEBUGF("Patch::connect(" << con1 << ", " << con2 << ")");

    if (!con1 || !con2) return 0;
    if (con1->module()->patch() != this
        || con2->module()->patch() != this)
    {
        CSMOD_RT_ERROR("The Patch can not connect outside Modules");
        return 0;
    }

    if (isConnected(con1, con2))
    {
        CSMOD_RT_ERROR("Request for duplicate connection " << con1->longIdName()
                       << " -> " << con2->longIdName());
        return 0;
    }

    // create and bind a connection class
    auto c = new Connection(con1, con2);
    cons_.push_back(c);

    updateDspGraph();

    return c;
}

bool Patch::deleteConnection(Connection * con)
{
    CSMOD_DEBUGF("Patch::deleteConnection(" << con << ")");

    for (auto i = cons_.begin(); i!=cons_.end(); ++i)
    if (*i == con)
    {
        // update connector ties
        con->disconnect();

        // wipe out Connection
        delete *i;
        cons_.erase(i);

        updateDspGraph();

        return true;
    }

    return false;
}


// ------------ configuration --------

void Patch::setBlockSize(size_t size)
{
    CSMOD_DEBUGF("Patch::setBlockSize(" << size << ")");

    blockSize_ = size;

    for (auto m : modules_)
    {
        m->setBlockSize(blockSize_);
    }

    // do it again
    // XXX setBlockSize() updates the storage
    // for dsp input connectors. This needs to happen in
    // correct execution order. (Every dsp-input
    // needs to be setup after all dsp-outputs that lead to it)
    updateDspStorage_();

    // update patch audio in/out buffer size
    audioInBuffer_.resize(numChannelsIn_ * blockSize_);
    audioOutBuffer_.resize(numChannelsOut_ * blockSize_);

    // update in/out modules
    if (audioInModule_)
        audioInModule_->setAudioInput(numChannelsIn_, &audioInBuffer_[0]);
    if (audioOutModule_)
        audioOutModule_->setAudioOutput(numChannelsOut_, &audioOutBuffer_[0]);
}

void Patch::setSampleRate(size_t rate)
{
    CSMOD_DEBUGF("Patch::setSampleRate(" << rate << ")");

    sampleRate_ = rate;

    for (auto m : modules_)
    {
        m->setSampleRate(sampleRate_);
    }

    //updateDspStorage_();
}

void Patch::setNumChannels(size_t in, size_t out)
{
    CSMOD_DEBUGF("Patch::setNumChannels(" << in << ", " << out << ")");
    numChannelsIn_ = in;
    numChannelsOut_ = out;

    // update patch audio in/out buffer
    audioInBuffer_.resize(numChannelsIn_ * blockSize_);
    audioOutBuffer_.resize(numChannelsOut_ * blockSize_);

    // update in/out modules
    if (audioInModule_)
        audioInModule_->setAudioInput(numChannelsIn_, &audioInBuffer_[0]);
    if (audioOutModule_)
        audioOutModule_->setAudioOutput(numChannelsOut_, &audioOutBuffer_[0]);
}


// ------------ dsp related ----------

bool Patch::updateDspGraph()
{
    CSMOD_DEBUGF("Patch::updateDspGraph()");

    DspGraph graph;
    if (!graph.initFromPatch(this))
    {
        dspmodules_.clear();
        return false;
    }

    graph.getSortedModules(dspmodules_);

    // update the connector storage memory/pointers
    updateDspStorage_();

    // update patch in/out modules

    // XXX There will be multiple ins & outs later!!

    // find AudioIn module
    audioInModule_ = 0;
    for (auto &m : dspmodules_)
    {
        if ((audioInModule_ = dynamic_cast<MODULE::DSP::AudioIn*>(m)))
        {
            audioInModule_->setAudioInput(numChannelsIn_, &audioInBuffer_[0]);
            break;
        }
    }

    // find AudioOut module
    audioOutModule_ = 0;
    for (auto &m : dspmodules_)
    {
        if ((audioOutModule_ = dynamic_cast<MODULE::DSP::AudioOut*>(m)))
        {
            audioOutModule_->setAudioOutput(numChannelsOut_, &audioOutBuffer_[0]);
            break;
        }
    }

    return true;
}

void Patch::updateDspStorage_()
{
    CSMOD_DEBUGF("Patch::updateDspStorage_()");

    for (auto dsp : dspmodules_)
    {
        // XXX This is currently the way to update a Module's Connectors
        dsp->setBlockSize(blockSize_);
    }
}

// ------------ runtime --------------

void Patch::audio_callback(const csfloat * in, csfloat * out)
{
    if (!audioInBuffer_.empty())
    for (size_t i = 0; i<blockSize_ * numChannelsOut_; ++i)
        audioInBuffer_[i] = *in;

    dspStep();

    if (!audioOutBuffer_.empty())
    for (size_t i = 0; i<blockSize_ * numChannelsOut_; ++i)
        *out++ = audioOutBuffer_[i];

}

void Patch::dspStep()
{
    for (auto &m : dspmodules_)
    {
        m->updateInputs();
        m->step();
        m->dspStep();
    }
}


// ------------- debug ---------------

void Patch::debug_dump()
{
    std::cout << "---------- patch debug dump -------------"
              << "\nmodules      " << modules_.size()
              << "\ndspmodules   " << dspmodules_.size()
              << "\naudio in/out " << (audioInModule_!=0) << "/" << (audioOutModule_!=0)
              << "\nconnections  " << cons_.size()
              << "\n";
    for (auto i : modules_)
    {
        i->debug_dump();
    }
    for (auto i : cons_)
    {
        i->debug_dump();
    }
}


} // namespace CSMOD
