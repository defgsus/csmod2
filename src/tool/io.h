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

#ifndef CSMOD_TOOL_IO_H
#define CSMOD_TOOL_IO_H

#include <cinttypes>
#include <string>
//#include <QString>

namespace CSMOD {


/** @brief serializer and deserializer - abstract */
class Io
{
public:

    Io();
    virtual ~Io();

    // ------------------ sections ------------------

    /** Create a new (sub-)section. */
    virtual void newSection(const std::string& name) = 0;

    /** Returns if the current section matches @p name. */
    virtual bool isSection(const std::string& name) const = 0;

    /** Returns name of the current section. */
    virtual const std::string& section(const std::string& name) const = 0;

    /** Ends the current sub-section and goes to top section. */
    virtual void endSection() = 0;

    // ----------------- data write -----------------

    virtual Io& addValue(const std::string& key, int) = 0;

    // ----------------- data read ------------------


    // _______________ PRIVATE AREA _________________

};




} // namespace CSMOD

#endif // CSMOD_TOOL_IO_H
