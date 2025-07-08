#ifndef SEP_ENGINE_WRAPPER_H
#define SEP_ENGINE_WRAPPER_H

#ifdef SEP_WORKBENCH_DEMO
#define SEP_DEMO_MODE
#endif

// Minimal set of required headers
#include <memory>
#include <string>
#include <vector>

// GLM includes
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace sep
{

    // Forward declarations for both modes
    struct Pattern;
    struct QuantumState;
    enum class MemoryTierEnum;

    // Base class definitions that are common to both modes
    class Engine
    {
    public:
        Engine() = default;
        virtual ~Engine() = default;
        virtual bool initialize() = 0;
        virtual void shutdown() = 0;
        virtual void setCudaEnabled(bool enabled) = 0;
        virtual void setMetricsEnabled(bool enabled) = 0;
        virtual void setLogLevel(int level) = 0;
    };

    class CyclesRenderer
    {
    public:
        CyclesRenderer() = default;
        virtual ~CyclesRenderer() = default;
        virtual bool initialize() = 0;
        virtual void render() = 0;
        virtual void present() = 0;
        virtual bool shouldClose() = 0;

        virtual void setWindowTitle(const std::string& title) = 0;
        virtual void setWindowSize(int width, int height) = 0;
        virtual void setFullscreen(bool fullscreen) = 0;
        virtual void setVSync(bool vsync) = 0;
        virtual void setSamples(int samples) = 0;
        virtual void setDenoising(bool denoising) = 0;
        virtual void setDevice(const std::string& device) = 0;

        virtual bool hasKeyEvent() = 0;
        virtual unsigned char getLastKey() = 0;
        virtual bool hasMouseEvent() = 0;
        virtual void getLastMouseEvent(int& x, int& y, int& button) = 0;

        virtual void setColorMode(const std::string& mode) = 0;
        virtual void setEmissionMode(const std::string& mode) = 0;
        virtual void renderPatternState(const std::vector<glm::vec3>& patterns) = 0;
    };

#ifdef SEP_DEMO_MODE

    // Quantum state structure
    struct QuantumState
    {
        float coherence{0.0f};
        float stability{0.0f};
        float evolution_rate{0.1f};
        float energy_level{1.0f};
        float coupling_strength{0.5f};
        float generation{0};
        float dimensions[3]{1.0f, 1.0f, 1.0f};
    };

    // Memory tier enum
    enum class MemoryTierEnum
    {
        STM,
        MTM,
        LTM
    };

    // Pattern structure
    struct Pattern
    {
        std::string id;
        float coherence{0.0f};
        float stability{0.5f};
        MemoryTierEnum memory_tier{MemoryTierEnum::STM};
        glm::vec3 position{0.0f, 0.0f, 0.0f};
        QuantumState quantum_state;
    };

    // Demo implementations
    class DemoEngine final : public Engine
    {
    public:
        bool initialize() override { return true; }
        void shutdown() override {}
        void setCudaEnabled(bool enabled) override {}
        void setMetricsEnabled(bool enabled) override {}
        void setLogLevel(int level) override {}
    };

    class DemoCyclesRenderer final : public CyclesRenderer
    {
    public:
        bool initialize() override { return true; }
        void render() override {}
        void present() override {}
        bool shouldClose() override { return false; }

        void setWindowTitle(const std::string& title) override {}
        void setWindowSize(int width, int height) override {}
        void setFullscreen(bool fullscreen) override {}
        void setVSync(bool vsync) override {}
        void setSamples(int samples) override {}
        void setDenoising(bool denoising) override {}
        void setDevice(const std::string& device) override {}

        bool hasKeyEvent() override { return false; }
        unsigned char getLastKey() override { return 0; }
        bool hasMouseEvent() override { return false; }
        void getLastMouseEvent(int& x, int& y, int& button) override { x = y = button = 0; }

        void setColorMode(const std::string& mode) override {}
        void setEmissionMode(const std::string& mode) override {}
        void renderPatternState(const std::vector<glm::vec3>& patterns) override {}
    };

    // Factory functions
    inline std::unique_ptr<Engine> createEngine()
    {
        return std::unique_ptr<Engine>(new DemoEngine());
    }

    inline std::unique_ptr<CyclesRenderer> createRenderer()
    {
        return std::unique_ptr<CyclesRenderer>(new DemoCyclesRenderer());
    }

    // Quantum evolution namespace
    namespace quantum
    {
        inline void applySpike(Pattern& pattern, float input, float decay, float threshold)
        {
            pattern.quantum_state.coherence =
                std::min(1.0f, pattern.quantum_state.coherence + input - decay);
        }

        inline void hebbianUpdate(Pattern& pre, Pattern& post, float rate)
        {
            post.quantum_state.stability += rate * pre.quantum_state.coherence;
            post.quantum_state.stability = std::min(1.0f, post.quantum_state.stability);
        }
    }  // namespace quantum

#else
    // Non-demo mode declarations
    std::unique_ptr<Engine> createEngine();
    std::unique_ptr<CyclesRenderer> createRenderer();
#endif

}  // namespace sep

#endif  // SEP_ENGINE_WRAPPER_H