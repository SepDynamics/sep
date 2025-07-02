#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "blender/mesh_handler.h"
#include "blender/blender_types.h"
#include "common/result.h"
#include "quantum/types.h"

namespace sep::blender::test {

class MockMeshHandler : public MeshHandler {
public:
    MOCK_METHOD(common::Result, init, (Object* bl_object, Mesh* bl_mesh), (override));
    MOCK_METHOD(common::Result, update, (const quantum::PatternData& pattern_data), (override));
    MOCK_METHOD(MeshHandler::MeshMetrics, getMetrics, (), (const, override));
    MOCK_METHOD(common::Result, addCustomDataLayer, (const char* name, int type), (override));
    MOCK_METHOD(common::Result, removeCustomDataLayer, (const char* name), (override));
    MOCK_METHOD(bool, hasCustomDataLayer, (const char* name), (const, override));
    MOCK_METHOD(common::Result, setUniformFloatLayer, (const char* name, float value), (override));
    MOCK_METHOD(common::Result, applyDeformation, (const MeshHandler::DeformParams& params), (override));
    MOCK_METHOD(common::Result, generateHyperMesh, (const quantum::PatternData& pattern, int dimensions), (override));
};

} // namespace sep::blender::test