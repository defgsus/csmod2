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

class Container;


class Module
{
    public:

    Module(const std::string& className)
        :   className_(className),
            idName_   (className),
            name_     (className)
    { }

    virtual ~Module();

    // ------------- cloning --------------------------

    /** return a new fresh instance belonging to the container */
    virtual Module * cloneClass() const = 0;

    // -------------- info ----------------------------

    /** Returns the name of the derived class */
    const std::string& className() const { return className_; }

    /** Returns the <b>global unique</b> ID of the Module */
    const std::string& idName() const { return idName_; }

    /** returns the user-defined name */
    const std::string& name() const { return name_; }

    // ------------ basic settings --------------------

    /** set a new user name */
    void name(const std::string& name);

    // ------------ initializers ----------------------


    // ------------- connectors -----------------------

    /** return number of all connectors */
    size_t numConnectors() const { return cons_.size(); }

    /** return the specific connector or 0 if out of range */
    Connector * connector(size_t index) const { return (index<cons_.size())? cons_[index] : 0; }

    /** read access to all Connectors of this module. */
    const std::vector<Connector*>& connectors() const { return cons_; }

    // -------------- container -----------------------

    /** return the container this module belongs to */
    Container * container() const { return container_; }

    // _________________ PROTECTED ____________________

    protected:

    // ------------ initializers ----------------------

    // ------------- connectors -----------------------

    /** adds a new Connector instance.
        <p>ownership is taken.</p>
        <p>the given instance is returned by the function.</p> */
    Connector* add_(Connector * c);

    // -------------- container -----------------------

    /** the container this module belongs to */
    Container* container_;

    // __________________ PRIVATE _____________________

    private:

    /** simply wipes out all connectors */
    void deleteConnectors_();

    /** all Connectors of this module */
    std::vector<Connector*> cons_;

    std::string
    /** derived classes name */
        className_,
    /** patch unique id */
        idName_,
    /** user defineable name */
        name_;

};






class TestModule : public Module
{
    public:

    TestModule() : Module("TestModule")
    {
        add_(new AudioConnector(this, Connector::IN,  "audio_in", "audio in"));
        add_(new AudioConnector(this, Connector::OUT, "audio_out", "audio out"));
    }

    virtual Module * cloneClass() const { return new TestModule; }

};



} // namespace CSMOD

#endif // MOD_MODULE_H
