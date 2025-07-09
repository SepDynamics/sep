#pragma once
// Mock header for cycles_renderer.hpp
namespace sep
{
    namespace blender
    {

        class CyclesRenderer
        {
        public:
            CyclesRenderer() = default;
            ~CyclesRenderer() = default;

            void setRotation(float rotation) {}
            void setZoom(float zoom) {}
            void setWireframe(bool wireframe) {}
            void setColorMode(int mode) {}
            void setEmissionMode(int mode) {}
            void setRoughnessMode(int mode) {}
            void renderPatternState(void* state) {}
            void cycleColorMode() {}
        };

    }  // namespace blender
}  // namespace sep