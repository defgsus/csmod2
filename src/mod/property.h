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

#include "log.h"
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

// ######################### Properties ##############################

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


// ######################### ValueProperty<T> ##############################

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



// ######################### ListProperty<T> ##############################

template <class T>
class ListProperty : public Property
{
public:
    // ----------- ctor ------------

    ListProperty(const String& id, const String& name,
                 const std::vector<T>&      item_values,
                 const std::vector<String>& item_ids,
                 const std::vector<String>& item_names,
                 const T& value)
        :   Property(id, name),
            value_  (value),
            default_(value),
            values_ (item_values),
            ids_    (item_ids),
            names_  (item_names)
    { }

    const T& value() const { return value_; }
    void value(const T& value) { value_ = value; setChanged(); }

    T idToValue(const String& id);
    String valueToId(const T& value);

    // ----------- io --------------

    virtual bool store(Io * io) { return io->write("v", valueToId(value_)); }
    virtual bool restore(Io * io);

protected:
    T value_, default_;
    std::vector<T> values_;
    std::vector<String> ids_, names_;
};

template <class T>
T ListProperty<T>::idToValue(const String& id)
{
    for (size_t i=0; i<ids_.size(); ++i)
        if (id == ids_[i])
            return values_[i];
    return T();
}

template <class T>
String ListProperty<T>::valueToId(const T & value)
{
    for (size_t i=0; i<values_.size(); ++i)
        if (value == values_[i])
            return ids_[i];
    return "";
}

template <class T>
bool ListProperty<T>::restore(Io * io)
{
    String vstr;
    if (!io->read("v", vstr, valueToId(default_))) return false;

    for (size_t i=0; i<ids_.size(); ++i)
        if (vstr == ids_[i])
        {
            value_ = values_[i];
            return true;
        }

    CSMOD_IO_WARN("unknown ListProperty id '" << vstr << "'");

    return false;
}

} // namespace CSMOD

#endif // CSMOD_MOD_PROPERTY_H
