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

/** analyzer and executer of dsp graphs */
class DspGraph
{
public:
    // ------------- ctor ----------------

    DspGraph();
    ~DspGraph();

    // -------------- init ---------------

    void clear();

    bool initFromPatch(Patch * patch);

    // ------------ runtime --------------

    void dspStep();

    // ____________ PRIVATE ______________

private:

    void initMap_();

    bool traceGraph_();

    void dump_();

    Patch * patch_;

    struct ModuleEdge;
    struct ModuleInfo;
    std::map<DspModule*, ModuleInfo*> map_;

    /** modules in order of execution */
    std::vector<DspModule*> modules_;
};


} // namespace CSMOD


#endif // DSPGRAPH_H
