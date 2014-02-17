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

#ifndef CSMOD_MOD_CONNECTOR_H
#define CSMOD_MOD_CONNECTOR_H

#include <string>
#include <vector>

/** @todo What kind of string to use anyway? */
#include <QString>

#include "mod/base.h"

namespace CSMOD {


class Connector
{
    // needed to set idName_
    friend class Module;

    public:

    /** direction of Connectors */
    enum Direction
    {
        IN,
        OUT
    };

    Connector(Module * module, Direction dir,
              const std::string& idName, const std::string& name)
        :	module_	(module),
            dir_    (dir),
            idName_	(idName),
            name_	(name)
    { }
    virtual ~Connector();

    // ------ info ---------

    /** Module, this Connector belongs to */
    Module * module() const { return module_; }
    /** direction (input or output) */
    Direction dir() const { return dir_; }
    /** persistent global id */
    const std::string& idName() const { return idName_; }
    /** human-readable name */
    const std::string& name() const { return name_; }

    /** Returns moduleId.connectorId */
    std::string longIdName() const;
    /** Returns moduleName.connectorName */
    std::string longName() const;

    virtual bool isDsp() const { return false; }

    // -------- connections -------------

    /** Returns true if this connector and 'other' are connectable */
    virtual bool isConnectable(Connector * other) const = 0;

    /** Adds the Connector to the list of connections.
        (Only statistics, still needs a Connection in Patch) */
    virtual bool connectTo(Connector * con);
    /** Removes the Connector from the list of connections.
        (Only statistics, still needs a Connection in Patch) */
    virtual bool disconnectFrom(Connector * con);

    /** Sees if this Connector is connected to the another one  */
    bool isConnectedTo(Connector * con) const;
    /** Sees if this Connector is connected to the Module  */
    bool isConnectedTo(Module * module) const;

    /** Returns number of connections going in or out */
    size_t numConnections() const { return cons_.size(); }

    /** Returns number of Modules connected to */
    size_t numModules() const { return modules_.size(); }

    /** read access to the list of connected Connectors */
    const Connectors& connectors() const { return cons_; }

    /** read access to the list of connected modules */
    const Modules& modules() const { return modules_; }

    // ------------- values --------------

    /** Reimplement and return textual representation of current value. */
    virtual QString valueQString() const { return "-"; }

    // --------------- runtime -----------

    /** When this is called, all outputs leading to this input
        are evaluated and can be summed up. */
    virtual void updateInputs() = 0;

    // ------------- debug ---------------

    virtual void debug_dump();

    // __________ PROTECTED ______________

    protected:

    /** parent module */
    Module * module_;

    /** direction in/out */
    Direction dir_;

    std::string
    /** persistent name */
        idName_,
    /** user defined name */
        name_;

    /** connected modules */
    Modules modules_;
    /** connected other Connectors */
    Connectors cons_;
};



class ValueConnector : public Connector
{
public:
    ValueConnector(Module * module, Direction dir,
                   const std::string& idname, const std::string& name)
        : Connector (module, dir, idname, name),
          value_    (0),
          userValue_(0)
    { }

    virtual bool isConnectable(Connector * other) const
    { return (dir() != other->dir() && dynamic_cast<ValueConnector*>(other) != 0); }

    // ------------- values --------------

    virtual QString valueQString() const { return QString::number(value_, 'g', 4); }

    csfloat value() const { return value_; }
    void value(csfloat v) { value_ = v; }

    csfloat userValue() const { return userValue_; }
    void userValue(csfloat v) { userValue_ = v; }

    // ----------- runtime ---------------

    /** Sums all inputs into value_ */
    virtual void updateInputs()
    {
        if (dir_ != IN) return;
        value_ = userValue_;
        for (auto c : cons_)
        {
            value_ += static_cast<ValueConnector*>(c)->value();
        }
    }

protected:
    csfloat value_, userValue_;
};



class DspConnector : public Connector
{
public:
    DspConnector(Module * module, Direction dir,
                 const std::string& idname, const std::string& name);

    // ------------ info ----------------

    bool isDsp() const { return true; }

    size_t blockSize() const { return blockSize_; }
    void setBlockSize(size_t size);

    virtual bool isConnectable(Connector * other) const
    { return (dir() != other->dir() && dynamic_cast<DspConnector*>(other) != 0); }

    // -------------- value -----------------

    /** Returns average of dsp block */
    virtual QString valueQString() const { return QString::number(blockAverage(),'g',4); }

    // ------------- runtime ----------------

    /** This will sum the dspblocks of multiple input Connectors.
        For outputs or single inputs this does nothing. */
    void updateInputs();

    /** read access to the dsp block */
    const csfloat * block() const { return dsp_block_ptr_; }
    /** write access to the dsp block */
    csfloat * block() { return dsp_block_ptr_; }

    /** Returns average of dsp block */
    csfloat blockAverage() const;

    // ------------- debug ---------------

    virtual void debug_dump();

protected:
    /** this will setup the internal or external block data. */
    virtual void updateDspStorage_();

    size_t blockSize_;
    csfloat * dsp_block_ptr_;
    csfloats dsp_block_;

    /** flag if inputs must be summed, or a single input
        is simply referenced */
    bool do_sum_dsp_inputs_;
};





} // namespace CSMOD

#endif // CSMOD_MOD_CONNECTOR_H
