#include <gtest/gtest.h>
#include "audio/config.h"
#include "audio/pipewire_capture.h"

#ifdef SEP_HAS_AUDIO

TEST(AudioHeaders, Compile)
{
    sep::audio::PipeWireCapture cap;
    sep::audio::AudioPipelineConfig cfg{};
    (void)cap;
    (void)cfg;
    SUCCEED();
}

#endif // SEP_HAS_AUDIO
