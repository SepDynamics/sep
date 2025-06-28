// Example corrected GPUContext usage without exceptions
#include "blender/gpu_context.h"

int main()
{
    sep::GPUContext ctx;
    if (ctx.init() != sep::SEPResult::SUCCESS)
    {
        return 1;
    }
    int count = 0;
    if (ctx.getDeviceCount(count) != sep::SEPResult::SUCCESS)
    {
        return 1;
    }
    return 0;
}
