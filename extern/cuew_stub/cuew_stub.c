#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* cuewErrorString(int code) {
    switch (code) {
        case 0:
            return "CUDA_SUCCESS";
        default:
            return "CUDA_ERROR";
    }
}

const char* cuewCompilerPath() { return "/usr/local/cuda/bin/nvcc"; }
int cuewCompilerVersion() { return 1200; }
int cuewInit(int flags) { (void)flags; return 0; }

#ifdef __cplusplus
}
#endif
