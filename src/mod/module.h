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

#ifndef MOD_MODULE_H
#define MOD_MODULE_H

#include "base.h"
#include "connector.h"
#include "connection.h"

#include <string>
#include <vector>

namespace CSMOD {

class Io;
class Module;

typedef int (*numModulesFunction)();
typedef Module * (*getModuleFunction)(int index);


class Container;
class Patch;

class Module
{
    // To adjust some private settings
    friend class Patch;

    public:

    /** Basic constructor of a Module.
        @p idName is the global unique persistent id of the Module.
        This is used to serialize patches and should never change
        for old pathch files to run.
        @p className is the global unique name of the class, used
        when loading the patch to determine the class to instantiate.
        <p>Both parameters get their whitespace or unknown chars
        replaced by an underscore.</p> */
    Module(const std::string& idName, const std::string& className);

    virtual ~Module();

    // ------------- cloning --------------------------

    /** return a new fresh instance belonging to the container */
    virtual Module * cloneClass() const = 0;

    // ------------------ IO -------------------

    virtual bool store(CSMOD::Io * io);
    virtual bool restore(CSMOD::Io * io);

    // -------------- info ----------------------------

    /** Returns the name of the derived class */
    const std::string& className() const { return className_; }

    /** Returns the <b>global unique</b> ID of the Module */
    const std::string& idName() const { return idName_; }

    /** returns the user-defined name */
    const std::string& name() const { return name_; }

    /** return Patch, this Module belongs to */
    Patch * patch() const { return patch_; }

    // ------------ basic settings --------------------

    /** set a new user name */
    void name(const std::string& name);

    // ------------ initializers ----------------------


    // ------------- connectors -----------------------

    /** Returns the Connector matching the id, or NULL if not present. */
    Connector * findConnector(const std::string& idName);

    /** Read-access to all Connectors of this module. */
    const std::vector<Connector*>& connectors() const { return cons_; }

    // ------------- connections ----------------------


    // -------------- container -----------------------

    /** return the container this module belongs to */
    Container * container() const { return container_; }

    // ------------- config ---------------------------

    /** Sets the sampling rate. This is only called
        once and then, so it's good to override this
        and do samplerate dependent pre-calculations. */
    virtual void setSampleRate(size_t rate) { sampleRate_ = rate; }
    size_t sampleRate() const { return sampleRate_; }

    // ---------------- runtime -----------------------

    /** actual worker. */
    virtual void step() { }

    // ------------- debug ---------------

    virtual void debug_dump();

    // _________________ PROTECTED ____________________

    protected:

    // ------------ initializers ----------------------

    // ------------- connectors -----------------------

    /** Adds a new Connector instance.
        <p>Ownership is taken.</p>
        <p>The given instance is returned by the function.</p>
        <p>The id's of the connector will be made unique for
        this module by appending or increasing digits if nescessary.</p> */
    virtual Connector* add_(Connector * c);

    // -------------- container -----------------------

    /** the container this module belongs to */
    Container* container_;

    // __________________ PRIVATE _____________________

    private:

    /** simply wipes out all connectors */
    void deleteConnectors_();

    // _______________ PRIVATE MEMBER _________________

    /** containing Patch */
    Patch * patch_;

    /** all Connectors of this module */
    std::vector<Connector*> cons_;

    std::string
    /** derived classes name */
        className_,
    /** patch unique id */
        idName_,
    /** user defineable name */
        name_;

    /** sampling rate of the containing patch. */
    size_t sampleRate_;

};





} // namespace CSMOD

#endif // MOD_MODULE_H
