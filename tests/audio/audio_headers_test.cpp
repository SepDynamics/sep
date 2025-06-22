#include <gtest/gtest.h>
#include "audio/config.h"
#include "audio/pipewire_capture.h"

TEST(AudioHeaders, Compile)
{
    sep::audio::PipeWireCapture cap;
    sep::audio::AudioPipelineConfig cfg{};
    (void)cap;
    (void)cfg;
    SUCCEED();
}
