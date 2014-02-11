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

#include "audiodevice.h"

#include <portaudio.h>

#include "log.h"
#include "audiodevices.h"

#define CSMOD_CHECKPA(command__, text__) \
    { PaError err__ = (command__); if (err__ != paNoError) \
        { CSMOD_DEV_ERROR(text__ << " (" << Pa_GetErrorText(err__) << ")"); return false; } }


namespace CSMOD {


// ------------ private portaudio specific data ---------

class AudioDevice::Private
{
public:
    PaStreamParameters inputParam, outputParam;
    PaStreamFlags streamFlags;
    PaStream *stream;
};

// ---------------- callback ----------------------------

// wrapper for the Pa callback to get private
// access to AudioDevice.
// We could register the Pa callback as a friend
// but that would require <portaudio.h> in the header file
inline int csmod_audio_callback_(AudioDevice * dev, const void * in, void * out)
{
    return dev->callback_(in, out);
}

// cannonical portaudio callback
static int csmod_pa_callback(
        const void    * inputBuffer,
        void          * outputBuffer,
        unsigned long /*framesPerBuffer*/,
        const PaStreamCallbackTimeInfo* /*timeInfo*/,
        PaStreamCallbackFlags /*statusFlags*/,
        void * userData )
{
    auto dev = static_cast<AudioDevice*>(userData);

    return csmod_audio_callback_(dev, inputBuffer, outputBuffer);
}


int AudioDevice::callback_(const void * in, void * out)
{
    // call user callback
    if (func_)
        func_(static_cast<const float*>(in), static_cast<float*>(out));

    return paContinue;
}




// ---------------- ctor -------------

AudioDevice::AudioDevice()
    :   deviceId_   (0),
        sampleRate_ (0),
        bufferLength_ (0),
        chanIn_     (0),
        chanOut_    (0),
        ok_         (0),
        play_       (0),
        func_       (0),
        p_          (new Private)
{
    CSMOD_DEBUGF("AudioDevice::AudioDevice()");
}

AudioDevice::~AudioDevice()
{
    CSMOD_DEBUGF("AudioDevice::~AudioDevice()");

    if (ok_) close();

    delete p_;
}



// ---------- info -------------------


// --------- initialisation ----------

bool AudioDevice::init(const Properties& props)
{
    return init(
        props.deviceIndex,
        props.numChannelsIn,
        props.numChannelsOut,
        props.sampleRate,
        props.bufferLength);
}

bool AudioDevice::init(size_t deviceIndex,
                       size_t numInputChannels,
                       size_t numOutputChannels,
                       size_t sampleRate,
                       size_t bufferLength)
{
    // init portaudio if not already done
    if (!AudioDevices::pa_initialized_)
    {
        CSMOD_CHECKPA(Pa_Initialize(), "could not initialize audio library");
        AudioDevices::pa_initialized_ = true;
    }

    // close previous session
    if (ok_) close();

    // get some info of device
    auto painf = Pa_GetDeviceInfo(deviceId_);
    if (!painf)
    {
        CSMOD_DEV_ERROR("can not get device info for device " << deviceId_);
        return false;
    }

    // setup in/out params

    PaStreamParameters * pap_in = 0;
    if (numInputChannels)
    {
        p_->inputParam.device = deviceIndex;
        p_->inputParam.channelCount = numOutputChannels;
        p_->inputParam.sampleFormat = paFloat32;
        p_->inputParam.suggestedLatency = painf->defaultLowInputLatency;
        p_->inputParam.hostApiSpecificStreamInfo = 0;
        pap_in = &p_->inputParam;
    }

    PaStreamParameters * pap_out = 0;
    if (numOutputChannels)
    {
        p_->outputParam.device = deviceIndex;
        p_->outputParam.channelCount = numOutputChannels;
        p_->outputParam.sampleFormat = paFloat32;
        p_->outputParam.suggestedLatency = painf->defaultLowOutputLatency;
        p_->outputParam.hostApiSpecificStreamInfo = 0;
        pap_out = &p_->outputParam;
    }

    // ---- check default parameters ---

    if (sampleRate == 0)
        sampleRate = painf->defaultSampleRate;

    if (bufferLength == 0)
        bufferLength = p_->outputParam.suggestedLatency * sampleRate;

    // --- stream flags ----

    p_->streamFlags = 0;

    // ---- open stream ----

    CSMOD_DEBUGDEV("opening audio stream"
                   << "\ndevice     " << deviceIndex << " " << painf->name
                   << "\nsamplerate " << sampleRate
                   << "\nbufferLength " << bufferLength
                   << "\nchannels   " << numInputChannels << " / " << numOutputChannels
                   );

    CSMOD_CHECKPA(
    Pa_OpenStream(&p_->stream,
                  pap_in,
                  pap_out,
                  sampleRate,
                  bufferLength,
                  p_->streamFlags,
                  csmod_pa_callback,
                  static_cast<void*>(this)
                  ),
        "could not init audiodevice '" << painf->name << "'");

    // store parameters

    name_ = painf->name;
    deviceId_ = deviceIndex;
    chanIn_ = numInputChannels;
    chanOut_ = numOutputChannels;
    sampleRate_ = sampleRate;
    bufferLength_ = bufferLength;

    CSMOD_DEBUGDEV("audio ok.");
    return ok_ = true;
}


bool AudioDevice::close()
{
    // no error reporting nescessary, i think
    if (!ok_) return false;

    if (play_)
    {
        if (!stop()) return false;
        // stop will tell the error
    }

    // XXX this error check is somewhat strange
    // but if we can't close the device,
    // we break here and leave the ok() flag on
    CSMOD_CHECKPA(
        Pa_CloseStream(p_->stream),
        "could not close audio device '" << name_ << "'");

    // reset params
    ok_ = play_ = false;

    return true;
}


void AudioDevice::setCallback(Callback func)
{
    func_ = func;
}


// ------------- runtime -------------

bool AudioDevice::start()
{
    if (!ok_) return false;
    CSMOD_CHECKPA(Pa_StartStream(p_->stream), "could not start audio stream");
    play_ = true;
    return true;
}

bool AudioDevice::stop()
{
    if (!(ok_ && play_)) return false;
    CSMOD_CHECKPA(Pa_StopStream(p_->stream), "could not stop audio stream");
    play_ = false;
    return true;
}






#undef CSMOD_CHECKPA

} // namespace CSMOD
