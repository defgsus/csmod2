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

#ifndef DSPGRAPH_H
#define DSPGRAPH_H

#include <vector>
#include <map>


namespace CSMOD {

class Patch;
class DspModule;
class Connection;
typedef std::vector<DspModule*> DspModules;

/** serializer of directed dsp graphs.
    Pass the graph (patch) to initFromPatch()
    and get the sorted order of Modules from getSortedModules().
    Afterwards the class can be destroyed.

    XXX This class could maybe be templated for other structures
        when the need arises..
*/
class DspGraph
{
public:
    // ------------- ctor ----------------

    DspGraph();
    ~DspGraph();

    // -------------- calc ---------------

    /** wipe out all contents */
    void clear();

    /** starts a new serialisation from the modules in patch.
        If false is returned, there was a loop in the graph. */
    bool initFromPatch(Patch * patch);

    /** initialize the vector with the dsp Modules in correct
        execution order. */
    void getSortedModules(DspModules& modules);

    // ____________ PRIVATE ______________

private:

    /** initialize local graph structure from patch */
    void initMap_();

    /** serialize the graph */
    bool traceGraph_();

    /** debug output */
    void dump_();

    /** connected patch */
    Patch * patch_;

    /** modules in order of execution */
    DspModules modules_;

    // local work structures
    struct ModuleEdge;
    struct ModuleNode;
    std::map<DspModule*, ModuleNode*> map_;
};


} // namespace CSMOD


#endif // DSPGRAPH_H
