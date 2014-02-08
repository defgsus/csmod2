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

#include "modulestock.h"

#include <dlfcn.h>

#include "log.h"
#include "mod/module.h"


namespace CSMOD {

ModuleStock * ModuleStock::instance_ = 0;

ModuleStock::ModuleStock()
{
    CSMOD_DEBUGF("ModuleStock::ModuleStock()");
}

ModuleStock::~ModuleStock()
{
    CSMOD_DEBUGF("ModuleStock::~ModuleStock()");
}

ModuleStock & ModuleStock::instance()
{
    if (!instance_)
        instance_ = new ModuleStock();

    return *instance_;
}

Module * ModuleStock::getModule(const std::string& idName)
{
    CSMOD_DEBUGF("ModuleStock::getModule(\"" << idName << "\")");
    auto i = modules_.find(idName);
    if (i == modules_.end())
    {
        CSMOD_RT_ERROR("request of unknown module '" << idName << "'");
        return 0;
    }
    return i->second->cloneClass();
}


bool ModuleStock::registerModule(Module * mod)
{
    CSMOD_DEBUGF("ModuleStock::registerModule(" << mod << ")");

    if (modules_.find(mod->idName()) != modules_.end())
    {
        CSMOD_RT_ERROR("module '" << mod << "' already registered.");
        return false;
    }

    modules_.insert(std::make_pair(mod->idName(), mod));

    return true;
}

bool ModuleStock::addDll(const std::string& filename)
{
    CSMOD_DEBUGF("ModuleStock::addDll(\"" << filename << "\")");

    void * handle = dlopen(filename.c_str(), RTLD_LAZY);
    if (!handle)
    {
        CSMOD_RT_ERROR("could not open dll '" << filename << "': "
                       << dlerror());
        return false;
    }

    dlerror();

    // --- test for compliance ---

    numModulesFunction numf;

    *(void **)(&numf) = dlsym(handle, "numberModules");

    if (!numf)
    {
        CSMOD_RT_ERROR("numberModules() not found in dll '" << filename << "': "
                       << dlerror());
        dlclose(handle);
        return false;
    }

    // keep this lib
    dlls_.push_back(handle);
    return true;
}



} // namespace CSMOD
