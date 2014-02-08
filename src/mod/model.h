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

#include <string>
#include <vector>

class PatchView;

namespace CSMOD {



class Model;
class Patch;
class Module;
class Connector;
class Connection;



/** @brief class for threadsafe editing.

    <p>The idea is as following. Model can be asked from
    anywhere to make changes to a Patch and will automatically
    signal the GUI/PatchView of these changes.</p>

    <p>That means, that even Patch should ask Model to make
    changes (If Patch needs to, anyway).</p>

    <p>It's also planned to record the undo history here.</p>
*/
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

    // -|-|-|-|-|-|-|- EDIT INTERFACE -|-|-|-|-|-|-|-

    // -------- module handling -----------

    /** Creates a Module in the specified patch. */
    bool createModule(Patch * patch, const std::string& idName);

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
