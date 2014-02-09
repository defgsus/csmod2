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

#ifndef CSMOD_AUDIO_AUDIODEVICE_H
#define CSMOD_AUDIO_AUDIODEVICE_H

#include <string>
#include <vector>
#include <iostream>

namespace CSMOD {

/** @brief Information center and initialiser for audio devices.

    Currently simply wraps portaudio. */
class AudioDevices
{
    public:

    struct ApiInfo
    {
        std::string name;
        size_t
            numDevices;
    };

    struct DeviceInfo
    {
        std::string name;
        size_t
            numInputChannels,
            numOutputChannels;
    };

    // --------------- ctor --------------

    AudioDevices();
    ~AudioDevices();

    // ---------- info -------------------

    /** call this once */
    bool checkDevices();

    size_t numApis();
    const ApiInfo * getApiInfo(size_t index);

    size_t numDevices();
    const DeviceInfo * getDeviceInfo(size_t index);

    void dump_info(std::ostream& = std::cout);

    // --------- initialisation ----------

    // _________ HIDDEN AREA _____________

private:

    size_t numDevices_, numApis_;
    std::vector<DeviceInfo> dev_infos_;
    std::vector<ApiInfo> api_infos_;
};


void testAudioDevices();

} // namespace CSMOD


#endif // CSMOD_AUDIO_AUDIODEVICE_H
