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
    :   Module       ("Stuff~", "Stuff~"),
        phase_          (0.0)
{
    CSMOD_DEBUGF("Stuff::Stuff()");

    add_(amp_= new ValueConnector(this, Connector::IN, "amp", "amp"));
    amp_->userValue(1);
    add_(freq_= new ValueConnector(this, Connector::IN, "freq", "freq"));
    freq_->userValue(300);
    add_(in_  = new DspConnector(this, Connector::IN,  "phase",  "phase" ));
    add_(out_ = new DspConnector(this, Connector::OUT, "out", "out"));
}

void Stuff::dspStep()
{
    for (size_t i = 0; i < blockSize(); ++i)
    {
        out_->block()[i] = amp_->value() * sinf(6.283 * (phase_ + in_->block()[i]));

        phase_ += freq_->value() / sampleRate();

        // keep number in range
        if (phase_>1.0) phase_ -= 2.0; else
        if (phase_<-1.0) phase_ += 2.0;
    }
}


} // namespace DSP
} // namespace MODULE
} // namespace CSMOD
