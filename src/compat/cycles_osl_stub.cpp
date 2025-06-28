// cycles_osl_stub.cpp - Minimal OSL stub for resolving link dependencies
// Place in src/compat/cycles_osl_stub.cpp

#include <memory>
#include <string>
#include <vector>

// Define the CCL namespace if not already defined
#ifndef CCL_NAMESPACE_BEGIN
#define CCL_NAMESPACE_BEGIN namespace ccl {
#define CCL_NAMESPACE_END }
#endif

CCL_NAMESPACE_BEGIN

// Forward declarations to satisfy external references
class ShaderManager;
class ShaderGraph;
class ShaderNode;
class Scene;
class Progress;

// OSLShaderManager stub implementation
class OSLShaderManager {
public:
    OSLShaderManager() {}
    virtual ~OSLShaderManager() {}
    
    // Required virtual methods that might be referenced
    virtual void reset(Scene* /*scene*/) {}
    virtual bool compile(Scene* /*scene*/, ShaderGraph* /*graph*/, Progress& /*progress*/) { return true; }
    virtual void update(Scene* /*scene*/, Progress& /*progress*/) {}
    
    // Static methods that might be referenced
    static bool osl_compile(const std::string& /*inputfile*/, 
                          const std::string& /*outputfile*/) { 
        return true; 
    }
    
    static bool osl_query(const std::string& /*filepath*/) { 
        return true; 
    }
};

// OSLCompiler stub - often referenced in cycles_osl
class OSLCompiler {
public:
    OSLCompiler(ShaderManager* /*manager*/, 
                Scene* /*scene*/,
                ShaderGraph* /*graph*/) {}
    
    void compile(Progress& /*progress*/) {}
    void add(ShaderNode* /*node*/, const char* /*name*/, bool /*isfilename*/ = false) {}
    
    // Parameter binding stubs
    void parameter(const char* /*name*/, float /*f*/) {}
    void parameter(const char* /*name*/, int /*i*/) {}
    void parameter(const char* /*name*/, const std::string& /*s*/) {}
    void parameter_color(const char* /*name*/, float /*r*/, float /*g*/, float /*b*/) {}
    void parameter_vector(const char* /*name*/, float /*x*/, float /*y*/, float /*z*/) {}
    void parameter_array(const char* /*name*/, const float* /*values*/, int /*count*/) {}
};

// OSL-specific node types that might be referenced
class OSLNode : public ShaderNode {
public:
    OSLNode() {}
    virtual ~OSLNode() {}
    
    std::string filepath;
    std::string bytecode_hash;
    bool has_surface_emission = false;
    bool has_surface_transparent = false;
    bool has_surface_bssrdf = false;
};

class OSLScriptNode : public OSLNode {
public:
    OSLScriptNode() {}
    virtual ~OSLScriptNode() {}
    
    std::string script;
    std::string bytecode;
};

// Global OSL initialization functions that might be referenced
void OSL_init() {}
void OSL_cleanup() {}

// Common OSL utility functions
namespace OSL {
    bool compile_shader(const std::string& /*src_file*/, 
                       const std::string& /*dst_file*/,
                       const std::vector<std::string>& /*options*/) {
        return true;
    }
    
    bool query_shader(const std::string& /*filename*/) {
        return true;
    }
}

CCL_NAMESPACE_END

// Export some commonly referenced symbols
extern "C" {
    // These might be referenced by name in some builds
    void cycles_osl_init() {}
    void cycles_osl_create_shader_manager() {}
    void cycles_osl_compile_shader() {}
}