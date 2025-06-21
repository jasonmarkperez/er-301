#include <hal/audio.h>
#include <hal/log.h>
#include "daisy_seed.h"
using namespace daisy;

extern "C" DaisySeed *Board_getDaisySeed();

static DaisySeed *seed;
static int rate = 48000;

static void DaisyAudioCallback(const float **in, float **out, size_t size)
{
    int buffer[AUDIO_NUM_CHANNELS * MAX_AUDIO_FRAME_LENGTH];
    for(size_t i = 0; i < size && i < MAX_AUDIO_FRAME_LENGTH; ++i)
    {
        // just pass zeros through; real implementation pending
        for(int ch=0; ch<AUDIO_NUM_CHANNELS; ++ch)
            buffer[AUDIO_NUM_CHANNELS*i+ch] = 0;
    }
    Audio_callback(buffer);
    for(size_t i = 0; i < size && i < MAX_AUDIO_FRAME_LENGTH; ++i)
    {
        out[0][i] = buffer[AUDIO_NUM_CHANNELS*i]/(float)AUDIO_MAX_OUTPUT_VALUE;
        if(AUDIO_NUM_CHANNELS>1)
            out[1][i] = buffer[AUDIO_NUM_CHANNELS*i+1]/(float)AUDIO_MAX_OUTPUT_VALUE;
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
