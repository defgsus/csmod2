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
    :   Module      (idName, className)
{
    CSMOD_DEBUGF("DspModule::DspModule(" << idName << ", " << className << ")");
}

Connector* DspModule::add_(Connector * c)
{
    Module::add_(c);
    if (c->dir() == Connector::IN)
    if (auto dsp = dynamic_cast<DspConnector*>(c))
    {
        dsp_inputs_.push_back(dsp);
    }
    return c;
}

void DspModule::setBlockSize(size_t size)
{
    Module::setBlockSize(size);

    // all dsp connectors
    for (auto &c : connectors())
    {
        auto dsp = dynamic_cast<DspConnector*>(c);
        if (dsp) dsp->setBlockSize(blockSize());
    }
}

void DspModule::updateDspInputs()
{
    for (auto c : dsp_inputs_)
    {
        c->transport();
    }
}


// ------------- debug ---------------

void DspModule::debug_dump()
{
    std::cout << "dspmodule " << this << " " << idName()
              << " blocks=" << blockSize() << "\n";
    for (auto i : connectors())
    {
        i->debug_dump();
    }
    std::cout << "\n";
}

} // namespace CSMOD
