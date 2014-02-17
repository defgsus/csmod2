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

#ifndef CSMOD_MOD_PROPERTY_H
#define CSMOD_MOD_PROPERTY_H

#include <string>
#include <vector>
#include <iostream>//debug

#include "tool/io.h"

namespace CSMOD {


class Property
{
    // for setting idName_
    friend class Properties;

public:
    // ----------- ctor ------------

    Property(const String& id, const String& name);

    virtual ~Property() { }

    // --------- info --------------

    const String& idName() const { return idName_; }
    const String& name() const { return name_; }

    bool changed() const { return changed_; }
    bool accept() { if (changed_) { changed_ = false; return true; } else return false; }
    void setChanged() { changed_ = true; }

    // ----------- io --------------

    virtual bool store(Io * io) = 0;
    virtual bool restore(Io * io) = 0;

private:

    String idName_, name_;

    bool changed_;
};


class Properties
{
public:
    ~Properties();

    // ------- managment -----------

    Property * add(Property * p);

    // -------- access -------------

    Property * find(const String& idName_);

    Property * operator[] (size_t index) { return props_[index]; }
    const Property * operator[] (size_t index) const { return props_[index]; }

    // ----------- io --------------

    bool store(Io * io);
    bool restore(Io * io);

private:
    std::vector<Property*> props_;
};



template <class T>
class ValueProperty : public Property
{
public:
    // ----------- ctor ------------

    ValueProperty(const String& id, const String& name,
                const T& value)
        :   Property(id, name),
            value_  (value),
            default_(value),
            limit_  (false)
    { }

    ValueProperty(const String& id, const String& name,
                const T& value, const T& minvalue, const T& maxvalue)
        :   Property(id, name),
            value_  (std::max(minvalue,std::min(maxvalue, value ))),
            min_    (minvalue),
            max_    (maxvalue),
            default_(value_),
            limit_  (true)
    { }

    virtual const T& value() const { return value_; }
    virtual void value(const T& value) { value_ = value; setChanged(); }

    // ----------- io --------------

    virtual bool store(Io * io) { return io->write("v", value_); }
    virtual bool restore(Io * io) { return io->read("v", value_, default_); }

protected:
    T value_, min_, max_, default_;
    bool limit_;
};



} // namespace CSMOD

#endif // CSMOD_MOD_PROPERTY_H
