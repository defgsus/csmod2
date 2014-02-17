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

#include "audioout.h"

#include "log.h"
#include "mod/modulestock.h"

namespace CSMOD {
namespace MODULE {
namespace DSP {


CSMOD_REGISTER_MODULE(AudioOut)
CSMOD_REGISTER_MODULE(AudioIn)

AudioOut::AudioOut()
    :   Module       ("AudioOut~", "AudioOut~"),
        numChannels_    (0),
        buffer_         (0)
{
    CSMOD_DEBUGF("AudioOut::AudioOut()");

    // XXX need to implemented dynamic Connectors
    ins_.resize(2);
    for (size_t i=0; i<ins_.size(); ++i)
        add_(ins_[i]  = new DspConnector(this, Connector::IN,  "in",  "in" ));
}

void AudioOut::setAudioOutput(size_t channels, csfloat *buffer)
{
    numChannels_ = channels;
    buffer_ = buffer;
}

void AudioOut::dspStep()
{
    if (!buffer_ || !numChannels_) return;

    auto b = &buffer_[0];
    for (size_t i = 0; i < blockSize(); ++i)
    for (size_t j = 0; j < numChannels_; ++j)
    {
        *b++ = ins_[j]->block()[i];
    }
}


AudioIn::AudioIn()
    :   Module       ("AudioIn~", "AudioIn~"),
        numChannels_    (0),
        buffer_         (0)
{
    CSMOD_DEBUGF("AudioIn::AudioIn()");

    // XXX need to implemented dynamic Connectors
    outs_.resize(2);
    for (size_t i=0; i<outs_.size(); ++i)
        add_(outs_[i]  = new DspConnector(this, Connector::OUT,  "out",  "out" ));
}

void AudioIn::setAudioInput(size_t channels, csfloat *buffer)
{
    numChannels_ = channels;
    buffer_ = buffer;
}

void AudioIn::dspStep()
{
    if (!buffer_ || !numChannels_) return;

    auto b = buffer_;
    for (size_t i = 0; i < blockSize(); ++i)
    for (size_t j = 0; j < numChannels_; ++j)
    {
        outs_[j]->block()[i] = *b++;
    }
}


} // namespace DSP
} // namespace MODULE
} // namespace CSMOD
