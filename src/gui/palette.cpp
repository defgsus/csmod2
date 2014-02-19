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

#include "palette.h"

namespace CSMOD {
namespace GUI {

Palette::Palette()
    :   default_color_  (200,200,200)
{
    setFactoryDefault();
}

// --------- colors -----------

void Palette::setFactoryDefault()
{
    set("patch.background",             QColor(30,30,40));
    set("patch.background.error",       QColor(70,30,20));
    set("connector.value",              QColor(100,100,100));
    set("connector.value.label",        QColor(155,170,155));
    set("connector.value.label.edit",   QColor(220,255,220));
    set("connector.dsp",                QColor(80,110,150));
    set("connector.dsp.label",          QColor(100,155,220));

    set("module",                       QColor(60,64,60));
    set("module.dsp",                   QColor(40,60,80));
}

/** color to return when id not found */
void Palette::setDefaultColor(const QColor& color)
{
    default_color_ = color;
}

/** Adds a new color group */
void Palette::set(const String& id, const QColor& color)
{
    map_[id] = color;
}

/** Returns the color for the group */
const QColor& Palette::get(const String& id) const
{
    auto i = map_.find(id);
    return (i != map_.end())? i->second : default_color_;
}

} // namespace GUI
} // namespace CSMOD
