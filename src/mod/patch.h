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

#ifndef CSMOD_MOD_PATCH_H
#define CSMOD_MOD_PATCH_H

#include <string>
#include <vector>

#include "base.h"

namespace CSMOD {

namespace MODULE { namespace DSP {
class AudioIn;
class AudioOut;
} }

/** (recursive) container for Modules and Connections */
class Patch
{
public:
    // ---------- types ------------------

    typedef std::vector<Module*> Modules;
    typedef std::vector<Connection*> Connections;
    typedef std::vector<Patch*> Patches;

    // -------------- ctor ---------------

    Patch();
    virtual ~Patch();

    // ------------------ IO -------------------

    virtual bool store(CSMOD::Io * io);
    virtual bool restore(CSMOD::Io * io);

    // ------------ info -----------------

    const std::string& idName() const { return idName_; }
    const std::string& name() const { return name_; }

    // ----------- handling --------------

    // --------- modules -----------------

    /** Returns Module by id, or NULL */
    Module * findModule(const std::string& idname);

    /** Sees if that instance is already there */
    bool hasModule(const Module * m);

    const Modules& modules() const { return modules_; }

    /** Adds the module and takes ownership.
        <p>If false is returned, there was something wrong
        and the Module was not added (ownership is not taken).
        This happens if the same Module is added again.</p>
        <p>The Module's idName will be made unique by
        adding counter digits as needed.</p> */
    bool addModule(Module * module);

    // ---------- connections ------------

    /** read access to all Connections in the patch. */
    const Connections& connections() const { return cons_; }

    /** Connects both connectors if possible */
    Connection * connect(Connector * con1, Connector * con2);

    /** Removes the given Connection.
        @p con will be deleted! */
    bool deleteConnection(Connection * con);

    bool isConnected(Connector * con1, Connector * con2) const;

    // ------------ configuration --------

    /** Sets the blocksize of this patch, as well as for
        all contained modules. This is the bufferlength
        of dsp blocks. */
    void setBlockSize(size_t size);

    /** Returns blocksize (dsp bufferlength) of this patch. */
    size_t blockSize() const { return blockSize_; }

    /** Sets the number of audio channels of the audio device. */
    void setNumChannels(size_t in, size_t out);

    size_t numChannelsIn() const { return numChannelsIn_; }
    size_t numChannelsOut() const { return numChannelsOut_; }

    /** Sets the sampling rate for this patch. */
    void setSampleRate(size_t rate);
    size_t sampleRate() const { return sampleRate_; }

    // ------------ dsp related ----------

    /** serializes all Modules to correct execution order. */
    bool updateDspGraph();

    // ------------ runtime --------------

    void audio_callback(const csfloat * in, csfloat * out);

    void dspStep();

    // ------------- debug ---------------

    void debug_dump();

    // ________ PROTECTED ________________

protected:

    // --------- dsp related -------------

    /** Calls Module::updateDspStorage() for each dsp Module
        in dspmodules_ */
    void updateDspStorage_();

    // ----------- id --------------------

    std::string
        idName_,
        name_;

    // ----------- items -----------------

    Modules modules_;
    Connections cons_;

    /** all dsp modules in execution order */
    Modules dspmodules_;

    // XXX there will be multiple ins and outs
    //std::vector<MODULE::DSP::AudioOut*>
    MODULE::DSP::AudioIn * audioInModule_;
    MODULE::DSP::AudioOut * audioOutModule_;
    csfloats audioInBuffer_,
             audioOutBuffer_;

    // ---------- configuration ----------

    /** dsp buffer length */
    size_t blockSize_,
    /** sampling rate (hz) */
           sampleRate_,
    /** input channels */
           numChannelsIn_,
    /** output channels */
           numChannelsOut_;
};

} // namespace CSMOD

#endif // CSMOD_MOD_PATCH_H
