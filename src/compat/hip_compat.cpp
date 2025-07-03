extern "C" {
struct hipDeviceProp_t;
typedef int hipError_t;
hipError_t hipGetDeviceProperties(hipDeviceProp_t* prop, int device);
hipError_t hipGetDevicePropertiesR0600(hipDeviceProp_t* prop, int device)
{
    return hipGetDeviceProperties(prop, device);
}
}
