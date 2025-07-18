#include "core/engine.h"
#include "gtest/gtest.h"
#include <fstream>

TEST(IngestionTest, DirectoryIngestion) {
    sep::core::Engine engine;
    // Create a temporary directory with some files
    std::filesystem::create_directory("test_dir");
    std::ofstream("test_dir/file1.txt") << "hello";
    std::ofstream("test_dir/file2.txt") << "world";

    engine.ingestFromDirectory("test_dir");

    // This is a placeholder for a real test.
    // We would need to check if the patterns were actually ingested.
    SUCCEED();

    // Clean up
    std::filesystem::remove_all("test_dir");
}

TEST(IngestionTest, SocketIngestion) {
    sep::core::Engine engine;
    // This is a placeholder for a real test.
    // We would need to create a socket and send some data to it.
    engine.ingestFromSocket(0);
    SUCCEED();
}