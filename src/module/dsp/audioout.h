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

#include "mod/dspmodule.h"

#ifndef CSMOD_MODULE_DSP_AUDIOOUT_H
#define CSMOD_MODULE_DSP_AUDIOOUT_H

namespace CSMOD {
namespace MODULE {
namespace DSP {

/* NOTE: Don't take this serious.
 * Currently i'm playing around with Modules to see
 * what patterns work best. */

class AudioOut : public DspModule
{
public:

    AudioOut();

    virtual AudioOut * cloneClass() const { return new AudioOut; }

    virtual void dspStep();

    void setAudioOutput(size_t channels, csfloat * buffer);

protected:

    DspConnectors ins_;
    size_t numChannels_;
    csfloat * buffer_;
};


class AudioIn : public DspModule
{
public:

    AudioIn();

    virtual AudioIn * cloneClass() const { return new AudioIn; }

    virtual void dspStep();

    void setAudioInput(size_t channels, csfloat * buffer);

protected:

    DspConnectors outs_;
    size_t numChannels_;
    csfloat * buffer_;
};

} // namespace DSP
} // namespace MODULE
} // namespace CSMOD

#endif // CSMOD_MODULE_DSP_AUDIOOUT_H
