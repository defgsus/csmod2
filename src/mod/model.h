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

#ifndef CSMOD_MOD_MODEL_H
#define CSMOD_MOD_MODEL_H

#include <vector>

class PatchView;

namespace CSMOD {

class Patch;
class Module;
class Connector;
class Connection;

/** @brief class for threadsafe editing */
class Model
{
public:
    // ---------------- ctor --------------

    Model();
    ~Model();

    // ------------- containers -----------

    /** set Patch to work on, or disconnect with NULL */
    void setPatch(Patch * root_patch);

    void addPatchView(PatchView * view);

    // -------- module handling -----------

    // -------- connection handling -------

    bool connect(Connector * from, Connector * to);

    bool disconnect(Connection * con);

private:

    void updateViews_();

    Patch * patch_;
    std::vector<PatchView*> views_;
};

} // namespace CSMOD


#endif // CSMOD_MOD_MODEL_H
