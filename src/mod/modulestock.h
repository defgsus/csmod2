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

#ifndef MODULESTOCK_H
#define MODULESTOCK_H

#include <string>
#include <vector>
#include <map>


#define CSMOD_REGISTER_MODULE( className__ ) \
    bool success_registering_##className__ = \
        ::CSMOD::ModuleStock::instance().registerModule( new className__ ); \

namespace CSMOD {

class Module;

/** @brief class containing all possible modules */
class ModuleStock
{
    // ------------------ ctor ----------------

    ModuleStock();
    ~ModuleStock();

public:

    /** singleton instance */
    static ModuleStock& instance();

    // ----------- module getter --------------

    /** return a fresh instance, or NULL */
    Module * getModule(const std::string& idName);

    /** add a dll to the stock */
    bool addDll(const std::string& filename);

    // ---------- register modules at runtime --

    /** Registers a Module. Ownership is taken.
        <p>return-value signifies success. although it's not irrelevant
        but it's mostly needed for the static-register trick.</p> */
    bool registerModule(Module * mod);

private:
    static ModuleStock * instance_;
    std::vector<void*> dlls_;
    std::map<std::string, Module*> modules_;
};

} // namespace CSMOD

#endif // MODULESTOCK_H

