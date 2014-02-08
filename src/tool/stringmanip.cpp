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

#include "stringmanip.h"

#include <cctype>
#include <iostream>

namespace CSMOD {


void increase_number(std::string& str, int init)
{
    if (str.empty()) return;

    // find last digit
    size_t end = str.length()-1;
    while (end && !std::isdigit(str[end])) --end;

    // no number found
    if (!std::isdigit(str[end]))
    {
        if (init<0) return;

        // add the init digit
        str += to_string(init);
        return;
    }

    // find start of number
    size_t start = end;
    while (start>1 && std::isdigit(str[start-1])) --start;

    // extract integer
    int value = from_string<int>(str.substr(start, end-start+1));

    value += 1;

    // replace number
    str.replace(start, end-start+1, to_string(value));
}



void checkIdName(std::string& str)
{
    for (auto i = str.begin(); i!=str.end(); ++i)
        if (*i <= ' ' || (unsigned char)*i >= 128)
            *i = '_';
}


} // namespace CSMOD
