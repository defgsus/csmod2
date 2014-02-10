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

#include "dspmodule.h"

#include "log.h"

namespace CSMOD {


DspModule::DspModule(const std::string& idName, const std::string& className)
    :   Module      (idName, className),
        blockSize_  (0)
{
    CSMOD_DEBUGF("DspModule::DspModule(" << idName << ", " << className << ")");
}


void DspModule::setBlockSize(size_t size)
{
    blockSize_ = size;

    // all dsp connectors
    for (auto c : connectors())
    {
        auto dsp = dynamic_cast<DspConnector*>(c);
        if (dsp) dsp->setBlockSize(blockSize_);
    }
}


} // namespace CSMOD
