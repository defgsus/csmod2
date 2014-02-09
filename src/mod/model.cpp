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

#include "model.h"

#include "log.h"
#include "tool/io.h"
#include "connector.h"
#include "module.h"
#include "patch.h"
#include "modulestock.h"

#include "gui/patchview.h"

namespace CSMOD {


Model::Model()
{
    CSMOD_DEBUGF("Model::Model()");
}

Model::~Model()
{
    CSMOD_DEBUGF("Model::~Model()");
}


// --------------- io -----------------

#define CSMOD_CHECKIO(command__, errortext__) \
    if (!command__) { CSMOD_IO_ERROR(errortext__); return false; }

bool Model::savePatch(const std::string& filename)
{
    CSMOD_DEBUGF("Model::savePatch(\"" << filename << "\")");

    CSMOD::Io io;
    CSMOD_CHECKIO(io.startWriting(), "could not initialize io");

    CSMOD_CHECKIO(patch_->store(&io), "could not store patch");

    for (auto v : views_)
    {
        CSMOD_CHECKIO(v->store(&io), "could not store view data");
    }

    CSMOD_CHECKIO(io.stopWriting(), "could not finalize io");

    CSMOD_CHECKIO(io.save(filename), "could not store xml");

    return true;
}


bool Model::loadPatch(const std::string& filename)
{
    CSMOD_DEBUGF("Model::loadPatch(\"" << filename << "\")");

    CSMOD::Io io;
    CSMOD_CHECKIO(io.load(filename), "could not load xml");

    CSMOD_CHECKIO(io.startReading(), "could not initialize io");

    CSMOD_CHECKIO(patch_->restore(&io), "could not load patch");

    updateViews_();

    for (auto v : views_)
    {
        CSMOD_CHECKIO(v->restore(&io), "could not load view data");
    }

    CSMOD_CHECKIO(io.stopReading(), "could not finalize io");

    return true;
}


// ------------- containers -----------

/** set Patch to work on, or disconnect with NULL */
void Model::setPatch(Patch * patch)
{
    CSMOD_DEBUGF("Model::setPatch(" << patch << ")");

    patch_ = patch;
}

void Model::addPatchView(PatchView * view)
{
    CSMOD_DEBUGF("Model::addPatchView(" << view << ")");

    views_.push_back(view);
}




// -------- module handling -----------

bool Model::createModule(Patch * patch, const std::string& idName)
{
    CSMOD_DEBUGF("Model::createModule(" << patch << ", \"" << idName << "\")");

    // get new instance
    auto m = ModuleStock::instance().getModule(idName);
    if (!m) return false;

    // add to patch
    if (!patch->addModule(m))
    {
        delete m;
        return false;
    }

    updateViews_();
    return true;
}


// -------- connection handling -------

bool Model::connect(Connector * from, Connector * to)
{
    CSMOD_DEBUGF("Model::connect(" << from << ", " << to << ")");

    if (!from || !to) return false;
    if (!from->isConnectable(to)) return false;

    if (!patch_->connect(from, to)) return false;

    updateViews_();
    return true;
}

bool Model::disconnect(Connection * con)
{
    CSMOD_DEBUGF("Model::Model(" << con << ")");

    if (!con || !patch_->disconnect(con)) return false;

    updateViews_();
    return true;
}







// ------------------ views -------------------------

void Model::updateViews_()
{
    CSMOD_DEBUGF("Model::updateViews_() (" << views_.size() << " views)");

    for (auto v : views_)
    {
        v->updateFromPatch();
        //v->updateCables();
    }
}

} // namespace CSMOD
