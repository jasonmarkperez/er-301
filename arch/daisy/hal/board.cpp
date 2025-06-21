#include <hal/board.h>
#include <hal/constants.h>
#include <hal/log.h>
#include "daisy_seed.h"
using namespace daisy;

static DaisySeed seed;

extern "C" {

DaisySeed *Board_getDaisySeed() { return &seed; }

void Board_init(void) {
    logInfo("Board_init: Daisy Seed");
    seed.Configure();
    seed.Init();
    seed.SetAudioBlockSize(MAX_AUDIO_FRAME_LENGTH);
}

void Board_enableI2C2(void) {}
void Board_enableUSB(void) {}
void Board_disableUSB(void) {}
void Board_pinmuxI2C2(void) {}
void Board_pinmuxUART0(void) {}

}
