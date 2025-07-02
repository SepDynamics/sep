#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "blender/base_types.h"
#include "blender/bridge.h"
#include "quantum/data.hpp"

namespace sep::test {

class MockBlenderBridge : public sep::pattern::BlenderBridge {
public:
    MOCK_METHOD(sep::SEPResult, init, (::sep::GPUContext* ctx), (override));
    MOCK_METHOD(sep::SEPResult, registerObject, (Object* obj, const sep::pattern::PatternConfig& config, sep::pattern::ObjectHandle* handle_out), (override));
    MOCK_METHOD(sep::SEPResult, updateObject, (sep::pattern::ObjectHandle handle, const sep::pattern::PatternMetrics& metrics), (override));
    MOCK_METHOD(sep::SEPResult, processPatterns, (), (override));
    MOCK_METHOD(sep::SEPResult, syncMemory, (::sep::memory::MemoryTierEnum tier, bool force), (override));
};

} // namespace sep::test