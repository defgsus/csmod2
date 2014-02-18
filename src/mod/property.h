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
#include "tool/stringmanip.h"
#include "gui/propertywidget.h"

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

    virtual bool changed() const = 0;
    virtual bool accept() = 0;

    // ----------- io --------------

    virtual bool store(Io * io) = 0;
    virtual bool restore(Io * io) = 0;

    // --------- gui ---------------

    virtual GUI::PropertyWidget * createWidget(QWidget * parent) = 0;

private:

    String idName_, name_;
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

    size_t size() const { return props_.size(); }
    Property * operator[] (size_t index) { return props_[index]; }
    const Property * operator[] (size_t index) const { return props_[index]; }

    // -------- info ---------------

    bool changed() const;

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
        :   Property (id, name),
            value_   (value),
            default_ (value),
            newvalue_(value),
            limit_   (false),
            changed_ (true)
    { }

    ValueProperty(const String& id, const String& name,
                const T& value, const T& minvalue, const T& maxvalue)
        :   Property (id, name),
            value_   (std::max(minvalue,std::min(maxvalue, value ))),
            min_     (minvalue),
            max_     (maxvalue),
            default_ (value_),
            newvalue_(value),
            limit_   (true),
            changed_ (true)
    { }

    virtual bool changed() const { return changed_; }
    virtual bool accept() { bool c = changed(); if (c) { changed_ = false; value_ = newvalue_; } return c; }

    virtual const T& value() const { return value_; }
    virtual void value(const T& value) { if (value != value_) { newvalue_ = value; changed_ = true; } }

    // ----------- io --------------

    virtual bool store(Io * io) { return io->write("v", value_); }
    virtual bool restore(Io * io) { return io->read("v", newvalue_, default_); changed_ = true; }

    // ------------ gui ------------

    virtual GUI::PropertyWidget * createWidget(QWidget * parent);

protected:
    T value_, min_, max_, default_, newvalue_;
    bool limit_;
    bool changed_;

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
            changed_(false),
            values_ (item_values),
            ids_    (item_ids),
            names_  (item_names)
    { }

    virtual bool changed() const { return changed_; }
    virtual bool accept() { bool c = changed(); if (c) { changed_ = false; value_ = newvalue_; } return c; }

    const T& value() const { return value_; }
    virtual void value(const T& value) { if (value != value_) { newvalue_ = value; changed_ = true; } }

    T idToValue(const String& id);
    String valueToId(const T& value);

    /** return index in values() for a value, or -1 if not found. */
    int indexOf(const T& value);

    /** Read access to the values associated to each item */
    const std::vector<T>& values() const { return values_; }

    /** Read access to the id strings associated to each item */
    const std::vector<String>& ids() const { return ids_; }

    /** Read access to the name strings associated to each item */
    const std::vector<String>& names() const { return names_; }

    // ----------- io --------------

    virtual bool store(Io * io) { return io->write("v", valueToId(value_)); }
    virtual bool restore(Io * io);

    // ------------ gui ------------

    virtual GUI::PropertyWidget * createWidget(QWidget * parent);

protected:
    T value_, default_, newvalue_;
    bool changed_;
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
int ListProperty<T>::indexOf(const T& value)
{
    for (size_t i=0; i<values_.size(); ++i)
        if (value == values_[i])
            return i;
    return -1;
}

template <class T>
bool ListProperty<T>::restore(Io * io)
{
    String vstr;
    if (!io->read("v", vstr, valueToId(default_))) return false;

    for (size_t i=0; i<ids_.size(); ++i)
        if (vstr == ids_[i])
        {
            newvalue_ = values_[i];
            changed_ = true;
            return true;
        }

    CSMOD_IO_WARN("unknown ListProperty id '" << vstr << "'");

    return false;
}

} // namespace CSMOD


// ############################ WIDGET IMPLEMENTATION #############################

#include "gui/propertywidget_impl.h"

namespace CSMOD {


template <class T>
GUI::PropertyWidget * ValueProperty<T>::createWidget(QWidget * parent)
{
    auto w = new GUI::ValuePropertyWidget<T>(this, parent);
    return w;
}

template <class T>
GUI::PropertyWidget * ListProperty<T>::createWidget(QWidget * parent)
{
    auto w = new GUI::ListPropertyWidget<T>(this, parent);
    return w;
}


} // namespace CSMOD

#endif // CSMOD_MOD_PROPERTY_H
