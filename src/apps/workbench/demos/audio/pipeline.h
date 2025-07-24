#pragma once

#include "capture.h"

namespace sep {
namespace audio {
    class Pipeline {
    public:
        void process(const float* fft_data, int fft_size) {}
    };
}
}