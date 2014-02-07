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

#include "mod/base.h"

namespace CSMOD {


class Module;



class Connector
{
    public:

    /** direction of Connectors */
    enum Direction
    {
        IN,
        OUT
    };

    Connector(Module * module, Direction dir,
              const std::string& idname, const std::string& name)
        :	module_	(module),
            dir_    (dir),
            idname_	(idname),
            name_	(name)
    { }

    // ------ getter ---------

    /** Module, this Connector belongs to */
    Module * module() const { return module_; }
    /** direction (input or output) */
    Direction dir() const { return dir_; }
    /** persistent global id */
    const std::string& idname() const { return idname_; }
    /** humand-readable name */
    const std::string& name() const { return name_; }

    // -------- connectors -------------

    /** return true if this connector and 'other' are connectable */
    virtual bool isConnectable(Connector * other) = 0;

    // __________ PRIVATE ______________

    private:

    /** parent module */
    Module * module_;

    /** direction in/out */
    Direction dir_;

    std::string
    /** persistent name */
        idname_,
    /** user defined name */
        name_;
};



class ValueConnector : public Connector
{
public:
    ValueConnector(Module * module, Direction dir,
                   const std::string& idname, const std::string& name)
        : Connector(module, dir, idname, name)
    { }

    virtual bool isConnectable(Connector * other)
    { return (dir() != other->dir() && dynamic_cast<ValueConnector*>(other) != 0); }

    csfloat value() const { return value_; }
    void value(csfloat v) { value_ = v; }

protected:
    csfloat value_;
};



class AudioConnector : public Connector
{
public:
    AudioConnector(Module * module, Direction dir,
                   const std::string& idname, const std::string& name)
        : Connector(module, dir, idname, name)
    { }

    virtual bool isConnectable(Connector * other)
    { return (dir() != other->dir() && dynamic_cast<AudioConnector*>(other) != 0); }

};





} // namespace CSMOD

#endif // CSMOD_MOD_CONNECTOR_H
