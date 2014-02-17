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
class Property;
class Properties;

typedef int (*numModulesFunction)();
typedef Module * (*getModuleFunction)(int index);


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
    Module(const String& idName, const String& className);

    virtual ~Module();

    // ------------- cloning --------------------------

    /** return a new fresh instance belonging to the container */
    virtual Module * cloneClass() const = 0;

    // ------------------ IO -------------------

    virtual bool store(CSMOD::Io * io);
    virtual bool restore(CSMOD::Io * io);

    // -------------- info ----------------------------

    /** Returns the name of the derived class */
    const String& className() const { return className_; }

    /** Returns the <b>global unique</b> ID of the Module */
    const String& idName() const { return idName_; }

    /** returns the user-defined name */
    const String& name() const;

    /** return Patch, this Module belongs to */
    Patch * patch() const { return patch_; }

    // --------- types / usability --------------------

    /** Returns true when this Module contains DspConnectors */
    bool isDsp() const { return is_dsp_; }

    // ------------ basic settings --------------------

    /** set a new user name */
    void setName(const String& name);

    // ------------ initializers ----------------------


    // ------------- connectors -----------------------

    /** Returns the Connector matching the id, or NULL if not present. */
    Connector * findConnector(const String& idName);

    /** Read-access to all Connectors of this module. */
    const std::vector<Connector*>& connectors() const { return cons_; }

    // ------------- connections ----------------------

    // --------------- properties ---------------------

    Properties& properties() { return *props_; }
    const Properties& properties() const { return *props_; }

    /** Called after a property change, or after restore.
        Check the Property::accept() flag to react to changes. */
    virtual void applyProperties();

    // ------------- config ---------------------------

    /** Sets the sampling rate. This is only called
        once and then, so it's good to override this
        and do samplerate dependent pre-calculations. */
    virtual void setSampleRate(size_t rate) { sampleRate_ = rate; }
    size_t sampleRate() const { return sampleRate_; }

    /** Sets the number of samples per dsp-block.
        Although only dsp modules depend on this setting,
        also normal Modules know the value. */
    virtual void setBlockSize(size_t size);
    size_t blockSize() const { return blockSize_; }

    // ---------------- runtime -----------------------

    /** Sums up value inputs or prepares storage (if any) in input DspConnectors.
        This will be called before each step()/dspStep(). */
    virtual void updateInputs();

    /** execute action for one dsp block */
    virtual void dspStep() = 0;

    /** XXX maybe between dsp steps? */
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

    /** simply wipes out all connectors */
    void deleteConnectors_();

    // -------------------- properties ----------------

    virtual Property* add_(Property * p);

    // __________________ PRIVATE _____________________

    private:

    // _______________ PRIVATE MEMBER _________________

    /** containing Patch */
    Patch * patch_;

    /** all Connectors of this module */
    std::vector<Connector*> cons_;
    /** Connector inputs that need to be summed */
    std::vector<Connector*> inputs_;

    String
    /** derived classes name */
        className_,
    /** patch unique id */
        idName_;

    // -------- properties -------

    /** list of all properties */
    Properties * props_;

    StringProperty * name_;

    // -------- config -----------

    bool is_dsp_;

    /** sampling rate of the containing patch. */
    size_t sampleRate_,
    /** number of samples per dsp-block. */
            blockSize_;

};





} // namespace CSMOD

#endif // MOD_MODULE_H
