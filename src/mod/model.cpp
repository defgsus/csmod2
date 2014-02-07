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
#include "connector.h"
#include "module.h"
#include "patch.h"

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
    }
}

} // namespace CSMOD
