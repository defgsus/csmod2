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

/** Macro for registering a Module at runtime.
    <p>Place this somewhere after your class definition:</p>
    @code
    class SuperDuper : public Module
    {
        ...
    };

    CSMOD_REGISTER_MODULE(SuperDuper)
    @endcode
    @note The id in Module::idName() is set from
    the constructor of your derived class. It must be
    <b>globally unique</b>, since no two Modules with
    the same id can be registered!
*/
#define CSMOD_REGISTER_MODULE( Class__ ) \
    bool success_registering_##Class__ = \
        ::CSMOD::ModuleStock::instance().registerModule( new Class__ ); \


namespace CSMOD {

class Module;

/** @brief class containing all possible modules.

    <p>This is a singleton class, all requests go to the
    same instance.</p>

    <p>The Modules, either loaded from a shared lib or
    registered with CSMOD_REGISTER_MODULE, are each constructed
    and wait in the stock for requests to copy themselves.
    This is not the most memory-efficient way, but makes compiled-in
    Modules easy to register.</p>

    <p>Modules from a shared library can possibly be constructed
    as needed. They only need to expose their idName prior to
    construction.</p>
*/
class ModuleStock
{
    // ------------------ ctor ----------------

    ModuleStock();
    ~ModuleStock();

public:

    /** singleton instance */
    static ModuleStock& instance();

    // ----------- module getter --------------

    /** Returns the number of registered modules. */
    size_t numModules() const { return modules_.size(); }

    /** Returns a fresh instance, or NULL if not found. */
    Module * getModule(const std::string& idName);

    /** Returns a pointer to the module instance without
        making a copy, or NULL if not found. */
    const Module * inspectModule(const std::string& idName) const;

    /** pushes all idNames on the vector. */
    void getIdNames(std::vector<std::string>& idNames) const;

    // ---------- register modules at runtime --

    /** add a dll to the stock */
    bool addDll(const std::string& filename);

    /** Registers a Module. Ownership is taken.
        <p>return-value signifies success. although it's not irrelevant
        but it's mostly needed for the static-register trick.</p> */
    bool registerModule(Module * mod);

private:

    /** singleton instance of this class. */
    static ModuleStock * instance_;

    /** currently: handles returned by dlopen().
       This will probably change for crossplatform use. */
    std::vector<void*> dlls_;

    /** List of all constructed modules, ready to copy themselves. */
    std::map<std::string, Module*> modules_;
};

} // namespace CSMOD

#endif // MODULESTOCK_H

