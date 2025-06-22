#include "context/processor.h"
#include "src/core/system_hooks.h"
#include <gtest/gtest.h>

using namespace sep::context;

class DummyHooks : public sep::core::SystemHooks {};

TEST(ContextProcessorFactory, CreatesProcessorWithHooks) {
    ProcessOptions opts;
    auto processor = createProcessor(opts);
    ASSERT_TRUE(processor);
    DummyHooks hooks;
    processor->setHooks(&hooks);
    SUCCEED();
}
