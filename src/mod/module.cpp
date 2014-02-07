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

namespace CSMOD {

Module::Module(const std::string& className)
    :   patch_    (0),
        className_(className),
        idName_   (className),
        name_     (className)
{

}

Module::~Module()
{
    deleteConnectors_();
}


// ------------------- connectors ---------------------

Connector* Module::add_(Connector * c)
{
    cons_.push_back(c);
    return c;
}

void Module::deleteConnectors_()
{
    for (auto c : cons_)
        delete c;
    cons_.clear();
}



} // namespace CSMOD
