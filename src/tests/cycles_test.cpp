#include "blender/cycles_renderer.h"
#include "quantum/data.hpp"
#include "core/common.h"

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// Generate a set of test patterns
std::vector<sep::pattern::PatternData> generateTestPatterns(int count) {
    std::vector<sep::pattern::PatternData> patterns;
    
    for (int i = 0; i < count; ++i) {
        sep::pattern::PatternData pattern;
        
        // Calculate normalized position in the pattern sequence
        float t = static_cast<float>(i) / count;
        
        // Generate interesting pattern properties based on position
        pattern.coherence = 0.5f + 0.5f * std::sin(t * 6.28f);
        pattern.stability = 0.5f + 0.5f * std::cos(t * 4.18f);
        pattern.entropy = 0.3f + 0.4f * std::sin(t * 9.42f);
        
        // Set some additional properties
        pattern.generation = i;  // Use generation instead of evolution_steps
        pattern.mutation_count = static_cast<std::uint32_t>(10.0f * pattern.entropy);
        
        // Add some position data
        pattern.position = glm::vec4(
            std::sin(t * 3.14f) * 2.0f,  // x
            std::cos(t * 3.14f) * 2.0f,  // y
            t * 2.0f - 1.0f,             // z
            1.0f                         // w
        );
        
        patterns.push_back(pattern);
    }
    
    return patterns;
}

int main(int argc, char* argv[]) {
    std::cout << "SEP Engine - Cycles Renderer Test" << std::endl;
    
    // Create the Cycles renderer
    sep::blender::CyclesRenderer renderer;
    
    // Check if Cycles support is available
    if (!sep::blender::CyclesRenderer::isCyclesAvailable()) {
        std::cout << "Cycles support is not available. Using stub implementation." << std::endl;
    } else {
        std::cout << "Cycles support is available." << std::endl;
    }
    
    // Initialize the renderer
    sep::SEPResult result = renderer.initialize();
    if (result != sep::SEPResult::SUCCESS) {
        std::cout << "Failed to initialize Cycles renderer." << std::endl;
        return 2;
    }
    
    std::cout << "Cycles renderer initialized successfully." << std::endl;
    
    // Generate test patterns
    std::cout << "Generating test patterns..." << std::endl;
    auto patterns = generateTestPatterns(20);
    std::cout << "Generated " << patterns.size() << " test patterns." << std::endl;
    
    // Create a scene from the patterns
    std::cout << "Creating scene from patterns..." << std::endl;
    result = renderer.createSceneFromPatterns(patterns);
    if (result != sep::SEPResult::SUCCESS) {
        std::cout << "Failed to create scene from patterns." << std::endl;
        return 3;
    }
    
    // Set up render parameters
    sep::blender::CyclesRenderer::RenderParams params;
    params.width = 640;
    params.height = 480;
    params.samples = 32;
    params.use_denoising = true;
    params.output_path = "cycles_render.ppm";
    
    // Parse command line arguments for output path
    if (argc > 1) {
        params.output_path = argv[1];
    }
    
    // Render the scene
    std::cout << "Rendering scene to " << params.output_path << "..." << std::endl;
    result = renderer.renderScene(params);
    if (result != sep::SEPResult::SUCCESS) {
        std::cout << "Failed to render scene." << std::endl;
        return 4;
    }
    
    std::cout << "Render completed successfully." << std::endl;
    std::cout << "Output file: " << params.output_path << std::endl;
    
    return 0;
}