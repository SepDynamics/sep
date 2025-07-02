if(SEP_HAS_CUDA)
  add_library(testbed_cuda_kernels STATIC increment_kernel.cu)
  set_property(TARGET testbed_cuda_kernels PROPERTY CUDA_STANDARD 17)
endif()
