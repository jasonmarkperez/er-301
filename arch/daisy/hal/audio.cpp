#include <hal/audio.h>
#include <hal/constants.h>
#include <hal/log.h>
#include "daisy_seed.h"
using namespace daisy;

extern "C" DaisySeed *Board_getDaisySeed();

static DaisySeed *seed;
static int rate = 48000;
static int int_buffer[AUDIO_NUM_CHANNELS * MAX_AUDIO_FRAME_LENGTH];

static void DaisyAudioCallback(const float **in, float **out, size_t size)
{
    if(size > MAX_AUDIO_FRAME_LENGTH) size = MAX_AUDIO_FRAME_LENGTH;

    // copy (and scale) input from Daisy to internal 24-bit integer buffer
    for(size_t i = 0; i < size; ++i)
    {
        for(int ch = 0; ch < AUDIO_NUM_CHANNELS; ++ch)
        {
            if(in && ch < 2 && in[ch])
                int_buffer[AUDIO_NUM_CHANNELS * i + ch] =
                    (int)(in[ch][i] * AUDIO_MAX_OUTPUT_VALUE);
            else
                int_buffer[AUDIO_NUM_CHANNELS * i + ch] = 0;
        }
    }

    Audio_callback(int_buffer);

    // mix down ER-301's 4 channels to stereo and convert back to float
    for(size_t i = 0; i < size; ++i)
    {
        int left  = int_buffer[AUDIO_NUM_CHANNELS * i] +
                     int_buffer[AUDIO_NUM_CHANNELS * i + 2];
        int right = int_buffer[AUDIO_NUM_CHANNELS * i + 1] +
                     int_buffer[AUDIO_NUM_CHANNELS * i + 3];

        out[0][i] = left / (float)AUDIO_MAX_OUTPUT_VALUE;
        out[1][i] = right / (float)AUDIO_MAX_OUTPUT_VALUE;
    }
}

extern "C" {

void Audio_init()
{
    logInfo("Audio_init: Daisy Seed");
    seed = Board_getDaisySeed();
    rate = seed->AudioSampleRate();
}

void Audio_start(void)
{
    seed->StartAudio(DaisyAudioCallback);
}

void Audio_stop(void)
{
    seed->StopAudio();
}

void Audio_restart(void)
{
    Audio_stop();
    Audio_start();
}

int Audio_getRate(void)
{
    return rate;
}

void Audio_printErrorStatus(void) {}
int Audio_getLoad() { return 0; }
bool Audio_running() { return seed && seed->AudioIsRunning(); }

}
