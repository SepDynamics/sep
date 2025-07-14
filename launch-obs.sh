#!/bin/bash
# Force OBS to use the GTX 1070 (GPU 1)
export __NV_PRIME_RENDER_OFFLOAD=1
export __GLX_VENDOR_LIBRARY_NAME=nvidia
export __VK_LAYER_NV_optimus=NVIDIA_only
export CUDA_VISIBLE_DEVICES=1


# Launch OBS
obs