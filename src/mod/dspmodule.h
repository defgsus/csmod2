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

#ifndef DSPMODULE_H
#define DSPMODULE_H

#include "module.h"


namespace CSMOD {

class DspModule : public Module
{
public:

    DspModule(const std::string& idName, const std::string& className);

    void setBlockSize(size_t size);

    size_t blockSize() const { return blockSize_ ; }

    /** execute action for one dsp block */
    virtual void dspStep() = 0;

private:

    size_t blockSize_;
};

typedef std::vector<DspModule*> DspModules;

} // namespace CSMOD

#endif // DSPMODULE_H
