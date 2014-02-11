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

#include "audiodevices.h"

#include <portaudio.h>

#include "log.h"
#include "audiodevice.h"


namespace CSMOD {


#define CSMOD_CHECKPA(command__, text__) \
    { PaError err__ = (command__); if (err__ != paNoError) \
        { CSMOD_DEV_ERROR(text__ << " (" << Pa_GetErrorText(err__) << ")"); return false; } }


bool AudioDevices::pa_initialized_ = false;

AudioDevices::AudioDevices()
{
    CSMOD_DEBUGF("AudioDevices::AudioDevices()");
}

AudioDevices::~AudioDevices()
{
    CSMOD_DEBUGF("AudioDevices::~AudioDevices()");
}


// ---------- info -------------------

bool AudioDevices::checkDevices()
{
    CSMOD_DEBUGF("AudioDevices::checkDevices()");

    if (!pa_initialized_)
    {
        CSMOD_CHECKPA(Pa_Initialize(), "could not initialize audio library");
        pa_initialized_ = true;
    }

    api_infos_.clear();
    dev_infos_.clear();

    // ----------- apis ------------------

    numApis_ = Pa_GetHostApiCount();

    for (size_t i=0; i<numApis_; ++i)
    {
        auto pa = Pa_GetHostApiInfo(i);

        ApiInfo inf;
        inf.name = pa->name;
        inf.numDevices = pa->deviceCount;

        api_infos_.push_back(inf);
    }

    // ----------- devices ---------------

    numDevices_ = Pa_GetDeviceCount();

    for (size_t i=0; i<numDevices_; ++i)
    {
        auto pa = Pa_GetDeviceInfo(i);

        DeviceInfo inf;
        inf.name = pa->name;
        inf.numInputChannels = pa->maxInputChannels;
        inf.numOutputChannels = pa->maxOutputChannels;
        inf.defaultSampleRate = pa->defaultSampleRate;
        inf.defaultBufferLength = pa->defaultLowOutputLatency
                                    * pa->defaultSampleRate;
        // sometimes Pa returns bogus values
        if (inf.defaultBufferLength > 4096)
            inf.defaultBufferLength = 4096;

        dev_infos_.push_back(inf);
    }

    return numDevices_ != 0;
}

size_t AudioDevices::numDevices() const
{
    return numDevices_;
}

const AudioDevices::DeviceInfo * AudioDevices::getDeviceInfo(size_t index) const
{
    if (index < dev_infos_.size())
        return &dev_infos_[index];

    return 0;
}

size_t AudioDevices::numApis() const
{
    return numApis_;
}

const AudioDevices::ApiInfo * AudioDevices::getApiInfo(size_t index) const
{
    if (index < api_infos_.size())
        return &api_infos_[index];

    return 0;
}

void AudioDevices::dump_info(std::ostream &out) const
{
    out << "audio APIs:\n";
    for (size_t i=0; i<numApis_; ++i)
    {
        out << "index    " << i << "\n"
            << "name     " << api_infos_[i].name << "\n"
            << "devices  " << api_infos_[i].numDevices << "\n\n";
    }

    out << "\naudio devices:\n";

    for (size_t i=0; i<numDevices_; ++i)
    {
        out << "index    " << i << "\n"
            << "name     " << dev_infos_[i].name << "\n"
            << "channels " << dev_infos_[i].numInputChannels
                    << " / " << dev_infos_[i].numOutputChannels << "\n\n";
    }

    out << std::endl;
}




#undef CSMOD_CHECKPA

} // namespace CSMOD

