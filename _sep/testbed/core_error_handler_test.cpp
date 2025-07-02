#include "core/error_handler.h"
#include <gtest/gtest.h>

TEST(ErrorHandlerTest, ReportAndRetrieveError) {
    auto &handler = sep::core::ErrorHandler::instance();
    handler.clearErrors();
    sep::Error err(sep::SEPResult::INVALID_ARGUMENT, "invalid arg");
    handler.reportError(err);
    auto errors = handler.getErrors();
    ASSERT_EQ(errors.size(), 1u);
    EXPECT_EQ(errors[0].code, sep::SEPResult::INVALID_ARGUMENT);
    EXPECT_STREQ(errors[0].message.c_str(), "invalid arg");
}

TEST(ErrorHandlerTest, HasErrorsFlag) {
    auto &handler = sep::core::ErrorHandler::instance();
    handler.clearErrors();
    EXPECT_FALSE(handler.hasErrors());
    handler.reportError(sep::Error(sep::SEPResult::UNKNOWN_ERROR, "oops"));
    EXPECT_TRUE(handler.hasErrors());
    handler.clearErrors();
    EXPECT_FALSE(handler.hasErrors());
}
