#include <gtest/gtest.h>
#include "apps/oanda_trader/oanda_trader_app.hpp"
#include <thread>
#include <chrono>

TEST(OandaTraderTest, AppLifecycle) {
    sep::apps::OandaTraderApp app;

    // Run the app in a separate thread for a short period
    std::thread app_thread([&]() {
        ASSERT_TRUE(app.initialize());
        app.run();
    });

    // Let the app run for a few seconds
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Signal the app to shut down
    app.shutdown();

    // Wait for the app thread to finish
    app_thread.join();

    SUCCEED();
}
