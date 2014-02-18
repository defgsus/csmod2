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

#include "module.h"

#include "log.h"
#include "tool/io.h"
#include "tool/stringmanip.h"
#include "property.h"

namespace CSMOD {

Module::Module(const String& idName, const String& className)
    :   patch_      (0),
        className_  (className),
        idName_     (idName),
        props_      (new Properties),
        is_dsp_     (false),
        sampleRate_ (0),
        blockSize_  (0)
{
    CSMOD_DEBUGF("Module::Module(\"" << idName << "\", \"" << className << "\")");

    // make sure the strings are right
    checkIdName(className_);
    checkIdName(idName_);

    add_( name_ = new StringProperty("name","module name", className_) );
}

Module::~Module()
{
    CSMOD_DEBUGF("Module::~Module()");

    deleteConnectors_();
}


// ------------------ IO -------------------

#define CSMOD_CHECKIO(command__, errortext__) \
    if (!(command__)) { CSMOD_IO_ERROR(errortext__); return false; }

bool Module::store(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Module::store(" << io << ")");

    CSMOD_CHECKIO(io->write("ver", 1), "could not write module version");
    io->write("class", className_);
    io->write("id", idName_);

    // store properties
    io->newSection("props");
    CSMOD_CHECKIO(props_->store(io), "could not store Properties");
    io->endSection();

    // store input ValueConnector values
    if (!connector_values_.empty())
    {
        io->newSection("values");
        for (auto m : connector_values_)
        {
            io->newSection("v");
            io->write("n", m.first);
            io->write("v", m.second);
            io->endSection();
        }
        io->endSection();
    }

    return true;
}

bool Module::restore(CSMOD::Io * io)
{
    CSMOD_DEBUGF("Module::restore(" << io << ")");

    int ver;
    CSMOD_CHECKIO(io->read("ver", ver), "could not read module version");
    CSMOD_CHECKIO(ver <= 1, "unknown module version " << ver);

    CSMOD_CHECKIO(io->read("id", idName_), "could not read module id");

    // restore properties
    while (io->nextSubSection())
    {
        CSMOD_DEBUGIO("Module::restore:: section = " << io->section());

        if (io->isSection("props"))
        {
            CSMOD_CHECKIO(props_->restore(io), "could not restore Properties");
        }
        else
        if (io->isSection("values"))
        {
            while (io->nextSubSection() && io->isSection("v"))
            {
                String name;
                csfloat value;
                CSMOD_CHECKIO(io->read("n", name), "could not read Connector value name");
                CSMOD_CHECKIO(io->read("v", value), "could not read Connector value");
                // add to list
                connector_values_.insert(std::make_pair(name, value));
                CSMOD_DEBUG("name = " << name << " value = " << value);
                io->leaveSection();
            }
        }

        io->leaveSection();
    }

    CSMOD_DEBUGIO("on end of module restore, section = " << io->section());

    return true;
}

#undef CSMOD_CHECKIO

// ------------------- info ---------------------------

const String& Module::name() const
{
    return name_->value();
}

// ------------------ config --------------------------

void Module::setName(const String &name)
{
    name_->value(name);
}

void Module::setBlockSize(size_t size)
{
    blockSize_ = size;

    // update all dsp connectors
    for (auto &c : connectors())
    {
        if (auto dsp = dynamic_cast<DspConnector*>(c))
            dsp->setBlockSize(blockSize());
    }
}

// ------------------- connectors ---------------------

Connector * Module::findConnector(const String& idName)
{
    for (auto c : cons_)
        if (idName == c->idName())
            return c;
    return 0;
}

Connector* Module::add_(Connector * c)
{
    // check for duplicate id
    if (findConnector(c->idName()))
    {
        String id(c->idName());
        do
        {
            increase_number(id,1);
        }
            while (findConnector(id));

        c->idName_ = id;
    }

    // keep in general list
    cons_.push_back(c);

    // inputs are kept additionally
    if (c->dir() == Connector::IN)
    {
        inputs_.push_back(c);
    }

    // check for dsp connectors
    is_dsp_ |= (dynamic_cast<DspConnector*>(c) != 0);

    return c;
}

void Module::deleteConnectors_()
{
    for (auto c : cons_)
        delete c;
    cons_.clear();
    inputs_.clear();
}


void Module::restoreConnectorValues_()
{
    CSMOD_DEBUGF("Module::restoreConnectorValues_()");

    for (auto &m : connector_values_)
    {
        auto c = findConnector(m.first);
        if (auto vc = dynamic_cast<ValueConnector*>(c))
        {
            vc->userValue(m.second);
        }
        else CSMOD_RT_WARN("Could not restore uservalue for " << m.first);
    }
}

void Module::setUserValue(const String& id, csfloat value)
{
    auto c = findConnector(id);
    if (!c) { CSMOD_RT_WARN("Module::setUserValue: unknown Connector " << id); return; }

    if (auto vc = dynamic_cast<ValueConnector*>(c))
    {
        vc->userValue(value);
        // store also in map
        connector_values_[id] = value;
    }
    else
        CSMOD_RT_WARN("Module::setUserValue: Connector " << id << " is no ValueConnector");
}

// ----------- properties ------------

Property* Module::add_(Property * p)
{
    props_->add(p);
    return p;
}

void Module::applyProperties()
{
    CSMOD_DEBUGF("Module::applyProperties() this=" << this << " / " << idName_);
}

// -------------- runtime ------------

void Module::updateInputs()
{
    for (auto c : inputs_)
    {
        c->updateInputs();
    }
}

// ------------- debug ---------------

void Module::debug_dump()
{
    std::cout << "--- module " << this << " " << idName()
              << " blocks=" << blockSize() << "\n";
    for (auto m : connector_values_)
        std::cout << "con=" << m.first << " uvalue=" << m.second << "\n";

    for (auto i : cons_)
        i->debug_dump();

    std::cout << std::endl;
}


} // namespace CSMOD
