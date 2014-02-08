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

#ifndef CSMOD_TOOL_STRINGDIGIT_H
#define CSMOD_TOOL_STRINGDIGIT_H

#include <string>
#include <sstream>

namespace CSMOD {


// ---------------- generic stuff -----------------

template <typename C>
void replace(std::basic_string<C>& str, C old_, C new_);

template <typename T>
std::string to_string(const T& value);

template <typename T>
T from_string(const std::string& str);

// --------------- id specific ---------------------

/** Increases the last number found in @p str.
    If there is no number in it, it will be set to @p init,
    if it is >= 0 */
void increase_number(std::string& str, int init = -1);

/** Correct idNames, e.g. replace spaces with _ */
void checkIdName(std::string& idName);

// ------------------ implementation -------------------------

template <typename T>
std::string to_string(const T& value)
{
    std::stringstream s;
    s << value;
    return s.str();
}

template <typename T>
T from_string(const std::string& str)
{
    std::stringstream s(str);
    T value;
    s >> value;
    return value;
}

template <typename C>
void replace(std::basic_string<C>& str, C old_, C new_)
{
    for (auto i = str.begin(); i!=str.end(); ++i)
        if (*i == old_) * i = new_;
}



} // namespace CSMOD

#endif // CSMOD_TOOL_STRINGDIGIT_H
