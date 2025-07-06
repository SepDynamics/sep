#include "audio/factory.h"
#include <gtest/gtest.h>

TEST(AudioFactoryTest, CreateAudioCaptureCompiles) {
    auto capture = sep::audio::createAudioCapture();
    (void)capture;
    SUCCEED();
}
