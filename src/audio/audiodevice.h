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

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <string>
#include <vector>
#include <functional>

namespace CSMOD {

/** @brief Wrapper around audio api

    Currently simply wraps portaudio.

    @code
    // example
    #include "audio/audiodevices.h"
    #include "audio/audiodevice.h"
    #include <functional>
    #include <math.h>

    void callback(const float * in, float * out,
                  size_t rate, size_t chan, size_t length)
    {
        // sample count
        static long int sam = 0;
        for (size_t i=0; i<length; ++i)
        {
            // ~200 hz tone
            float s = sinf( (float)sam++ * 6.28f * 200.f / rate );

            // fill each channel
            for (size_t j=0; j<chan; ++j)
                *out++ = s;
        }
    }

    void main()
    {
        using namespace CSMOD;
        using namespace std::placeholders;

        AudioDevices devs;
        devs.checkDevices();
        devs.dump_info();

        AudioDevice dev;
        // 0-device is commonly working
        dev.init(0, 2,2, 44100, 512);
        // bind some arguments to the callback
        dev.setCallback(std::bind(some, _1, _2,
                dev.sampleRate(), dev.numOutputChannels(), dev.bufferSize()));

        dev.start();

        sleep(1);

        dev.stop();
        dev.close();
    }

    @endcode
*/
class AudioDevice
{
    // access from the callback to this class
    friend int csmod_audio_callback_(AudioDevice*, const void*, void*);

public:

    // ------------ types ----------------

    /** The type of function issued by this class for
        audio callback. */
    typedef std::function<void(const float*,float*)> Callback;


    // --------------- ctor --------------

    AudioDevice();
    /** This will, if nescessary, stop and close the stream. */
    ~AudioDevice();

    // ---------- info -------------------

    /** Returns true when an audio stream is initialized */
    bool ok() const { return ok_; }
    /** Returns true when the audio stream is running/playing */
    bool playing() const { return play_; }

    /** name of the audio device */
    const std::string& name() const { return name_; }
    /** host-os index of the selected device */
    size_t deviceId() const { return deviceId_; }
    /** number of channel in input */
    size_t numInputChannels() const { return chanIn_; }
    /** number of channels to output */
    size_t numOutputChannels() const { return chanOut_; }
    /** samplerate in hertz (samples per second) */
    size_t sampleRate() const { return sampleRate_; }
    /** length of buffer in samples. */
    size_t bufferLength() const { return bufferLength_; }

    // --------- initialisation ----------

    /** Initializes a device/stream.
        @p deviceIndex is the host-os device number as returned by CSMOD::AudioDevices.
        @p numInputChannels and @p numOutputChannels is the number of channels requested
        for the input and output. One of them can be set to zero, if you do not need
        the particular stream.
        @p sampleRate sets the desired samples per second and can be left zero
        to use the device's default.
        @p bufferLength defines the desired samples in a buffer, and therefore the
        input/output latency. It can also be left zero to select the device's default.
        Note that a bufferlength of, e.g., 128 for two output channels means, that
        you actually need to generate 256 samples.
        @return Returns true for success. Subsequently you can use start() to start
        the audio stream.
        */
    bool init(size_t deviceIndex,
              size_t numInputChannels,
              size_t numOutputChannels,
              size_t sampleRate = 0,
              size_t bufferLength = 0);

    /** close the audio stream. */
    bool close();

    /** Installs a function that is called from the audio device for reading/filling
        the buffer. The callback is expected to look like this:
        @code
        void callback(const float * input, float * output);
        @endcode
        Of course you can use std::bind to link other stuff here.
        input holds numInputChannels() * bufferLength() floats and output
        has place for numOutputChannels() * bufferLength() floats.
        @note It's common practice to only do memory-shifting and calculation
        in this thread and not call any OS functions. Some say you should even
        avoid mutexes. */
    void setCallback(Callback func);

    // ------------- runtime -------------

    /** Starts the audio thread once the stream is initialized with init() */
    bool start();
    /** Temporarily stops the audio thread. Can be reenabled with start(). */
    bool stop();

    // _________ HIDDEN AREA _____________

private:

    int callback_(const void * in, void * out);

    size_t
        deviceId_,
        sampleRate_,
        bufferLength_,
        chanIn_,
        chanOut_;

    std::string name_;

    volatile bool ok_, play_;

    Callback func_;

    // backend library specific data
    class Private;
    Private * p_;
};


} // namespace CSMOD

#endif // AUDIODEVICE_H
