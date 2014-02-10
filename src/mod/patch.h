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

namespace CSMOD {

class Io;
class Module;
class Connector;
class Connection;

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
        If false is returned, there was something wrong
        and the Module was not added.
        The Module's idName will be made globally unique. */
    bool addModule(Module * module);

    // ---------- connections ------------

    const Connections& connections() const { return cons_; }

    /** Connects both connectors if possible */
    Connection * connect(Connector * con1, Connector * con2);

    /** Removes the given Connection */
    bool disconnect(Connection * con);

    // ------------ configuration --------

    /** set the blocksize of this patch, as well as for
        all contained modules. This is the bufferlength
        of dsp blocks. */
    void setBlockSize(size_t size);

    size_t blockSize() const { return blockSize_; }

    // ------------ runtime --------------

    void dspStep();

    // ________ PROTECTED ________________

protected:

    // ----------- id --------------------

    std::string
        idName_,
        name_;

    // ----------- items -----------------

    Modules modules_;
    Connections cons_;

    // ---------- configuration ----------

    /** dsp buffer length */
    size_t blockSize_;
};

} // namespace CSMOD

#endif // CSMOD_MOD_PATCH_H
