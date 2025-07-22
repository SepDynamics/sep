#pragma once

namespace sep {
namespace audio {
    class Capture {
    public:
        void start() {}
        void stop() {}
        const float* get_fft_data() { return nullptr; }
        int get_fft_size() { return 0; }
    };
}
}