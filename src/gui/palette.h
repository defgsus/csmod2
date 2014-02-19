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

#ifndef PALETTE_H
#define PALETTE_H

#include <map>

#include <QColor>

#include "base.h"

namespace CSMOD {
namespace GUI {

class Palette
{
public:
    /** Calls setFactoryDefault */
    Palette();

    // --------- colors -----------

    void setFactoryDefault();

    /** color to return when id not found */
    void setDefaultColor(const QColor& color);

    /** Adds a new color group */
    void set(const String& id, const QColor& color);

    /** Returns the color for the group */
    const QColor& get(const String& id) const;

private:

    std::map<String,QColor> map_;
    QColor default_color_;
};

} // namespace GUI
} // namespace CSMOD

#endif // PALETTE_H
