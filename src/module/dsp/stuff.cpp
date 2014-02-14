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

#include "stuff.h"

#include <cmath>

#include "log.h"
#include "mod/modulestock.h"

namespace CSMOD {
namespace MODULE {
namespace DSP {


CSMOD_REGISTER_MODULE(Stuff)


Stuff::Stuff()
    :   DspModule       ("Stuff~", "Stuff~"),
        phase_          (0.0)
{
    CSMOD_DEBUGF("Stuff::Stuff()");

    add_(in_  = new DspConnector(this, Connector::IN,  "in",  "in" ));
    add_(out_ = new DspConnector(this, Connector::OUT, "out", "out"));
}

void Stuff::dspStep()
{
    for (size_t i = 0; i < blockSize(); ++i)
    {
        out_->block()[i] = 0.5f * sinf(phase_);
        phase_ += 6.28 * 350.0 / sampleRate()
                + in_->block()[i];
    }
}


} // namespace DSP
} // namespace MODULE
} // namespace CSMOD
