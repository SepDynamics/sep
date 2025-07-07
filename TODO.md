im trying to get these tests to pass via maaking the codebase work.

[2024-04-25] Added clamping for tiny utilization values in
MemoryTierManager::getTierUtilization so that tests expecting exact zero
no longer fail due to floating point rounding.


help me bring this to functional   


cd /sep/sep_build/build &&  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14 -B . -S .. && make memory_manager_tests && ./memory_manager_tests
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /sep/sep_build/build
[ 62%] Built target sep_core
[ 87%] Built target sep_memory
[100%] Built target memory_manager_tests
Running main() from /builddir/build/BUILD/gtest-1.15.2-build/googletest-1.15.2/googletest/src/gtest_main.cc
[==========] Running 11 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 11 tests from MemoryTierManagerTest
[ RUN      ] MemoryTierManagerTest.BasicInitialization
[       OK ] MemoryTierManagerTest.BasicInitialization (0 ms)
[ RUN      ] MemoryTierManagerTest.AllocationAndDeallocation
[       OK ] MemoryTierManagerTest.AllocationAndDeallocation (0 ms)
[ RUN      ] MemoryTierManagerTest.PromotionAndDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:29: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:30: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::LTM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:39: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.PromotionAndDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:53: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:54: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::STM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:63: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:81: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:82: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::STM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:91: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.AllocationNearDefragmentBoundary
[       OK ] MemoryTierManagerTest.AllocationNearDefragmentBoundary (0 ms)
[ RUN      ] MemoryTierManagerTest.AutoDefragmentationThreshold
[       OK ] MemoryTierManagerTest.AutoDefragmentationThreshold (0 ms)
[ RUN      ] MemoryTierManagerTest.TotalMetrics
[       OK ] MemoryTierManagerTest.TotalMetrics (0 ms)
[ RUN      ] MemoryTierManagerTest.CalculateRelationshipCoherence
[       OK ] MemoryTierManagerTest.CalculateRelationshipCoherence (0 ms)
[ RUN      ] MemoryTierManagerTest.CleanupExpiredPatterns
[       OK ] MemoryTierManagerTest.CleanupExpiredPatterns (0 ms)
[ RUN      ] MemoryTierManagerTest.PrunePatternsByPriority
[       OK ] MemoryTierManagerTest.PrunePatternsByPriority (0 ms)
[----------] 11 tests from MemoryTierManagerTest (0 ms total)

[----------] Global test environment tear-down
[==========] 11 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 8 tests.
[  FAILED  ] 3 tests, listed below:
[  FAILED  ] MemoryTierManagerTest.PromotionAndDemotion
[  FAILED  ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion
[  FAILED  ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion

 3 FAILED TESTS
╭─ajn@sep-OS /sep/sep_build/build ‹master› 
╰─$ git merge -X ours cb8b06e8fd8dbf55b0bea24b0c2e68029b18fa10                     1 ↵
Auto-merging src/memory/memory_tier_manager.cpp
hint: Waiting for your editor to close the file... error: cannot run vim: No such file or directory
error: unable to start editor 'vim'
Not committing merge; use 'git commit' to complete the merge.
╭─ajn@sep-OS /sep/sep_build/build ‹master●› 
╰─$ cd /sep/sep_build/build &&  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14 -B . -S .. && make memory_manager_tests && ./memory_manager_tests
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /sep/sep_build/build
[ 62%] Built target sep_core
[ 87%] Built target sep_memory
[100%] Built target memory_manager_tests
Running main() from /builddir/build/BUILD/gtest-1.15.2-build/googletest-1.15.2/googletest/src/gtest_main.cc
[==========] Running 11 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 11 tests from MemoryTierManagerTest
[ RUN      ] MemoryTierManagerTest.BasicInitialization
[       OK ] MemoryTierManagerTest.BasicInitialization (0 ms)
[ RUN      ] MemoryTierManagerTest.AllocationAndDeallocation
[       OK ] MemoryTierManagerTest.AllocationAndDeallocation (0 ms)
[ RUN      ] MemoryTierManagerTest.PromotionAndDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:29: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:30: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::LTM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:39: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.PromotionAndDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:53: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:54: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::STM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:63: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:81: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:82: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::STM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:91: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.AllocationNearDefragmentBoundary
[       OK ] MemoryTierManagerTest.AllocationNearDefragmentBoundary (0 ms)
[ RUN      ] MemoryTierManagerTest.AutoDefragmentationThreshold
[       OK ] MemoryTierManagerTest.AutoDefragmentationThreshold (0 ms)
[ RUN      ] MemoryTierManagerTest.TotalMetrics
[       OK ] MemoryTierManagerTest.TotalMetrics (0 ms)
[ RUN      ] MemoryTierManagerTest.CalculateRelationshipCoherence
[       OK ] MemoryTierManagerTest.CalculateRelationshipCoherence (0 ms)
[ RUN      ] MemoryTierManagerTest.CleanupExpiredPatterns
[       OK ] MemoryTierManagerTest.CleanupExpiredPatterns (0 ms)
[ RUN      ] MemoryTierManagerTest.PrunePatternsByPriority
[       OK ] MemoryTierManagerTest.PrunePatternsByPriority (0 ms)
[----------] 11 tests from MemoryTierManagerTest (0 ms total)

[----------] Global test environment tear-down
[==========] 11 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 8 tests.
[  FAILED  ] 3 tests, listed below:
[  FAILED  ] MemoryTierManagerTest.PromotionAndDemotion
[  FAILED  ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion
[  FAILED  ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion

 3 FAILED TESTS
╭─ajn@sep-OS /sep/sep_build/build ‹master●› 
╰─$ git merge -X ours cacc9f0f6966c675297fb64add6db8721937482a                     1 ↵
Auto-merging src/memory/memory_tier_manager.cpp
hint: Waiting for your editor to close the file... error: cannot run vim: No such file or directory
error: unable to start editor 'vim'
Not committing merge; use 'git commit' to complete the merge.
╭─ajn@sep-OS /sep/sep_build/build ‹master●› 
╰─$ cd /sep/sep_build/build &&  cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-14 -B . -S .. && make memory_manager_tests && ./memory_manager_tests
-- Configuring done (0.0s)
-- Generating done (0.0s)
-- Build files have been written to: /sep/sep_build/build
[ 62%] Built target sep_core
[ 87%] Built target sep_memory
[100%] Built target memory_manager_tests
Running main() from /builddir/build/BUILD/gtest-1.15.2-build/googletest-1.15.2/googletest/src/gtest_main.cc
[==========] Running 11 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 11 tests from MemoryTierManagerTest
[ RUN      ] MemoryTierManagerTest.BasicInitialization
[       OK ] MemoryTierManagerTest.BasicInitialization (0 ms)
[ RUN      ] MemoryTierManagerTest.AllocationAndDeallocation
[       OK ] MemoryTierManagerTest.AllocationAndDeallocation (0 ms)
[ RUN      ] MemoryTierManagerTest.PromotionAndDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:29: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:30: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::LTM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:39: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.PromotionAndDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:53: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:54: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::STM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:63: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion
/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:81: Failure
Expected equality of these values:
  mgr.getTierUtilization(MemoryTierEnum::MTM)
    Which is: 0.000244140625
  0.0f
    Which is: 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:82: Failure
Expected: (mgr.getTierUtilization(MemoryTierEnum::STM)) > (0.0f), actual: 0 vs 0

/sep/sep_build/tests/memory/memory_tier_manager_test.cpp:91: Failure
Expected: (promoted) != (nullptr), actual: NULL vs (nullptr)

[  FAILED  ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion (0 ms)
[ RUN      ] MemoryTierManagerTest.AllocationNearDefragmentBoundary
[       OK ] MemoryTierManagerTest.AllocationNearDefragmentBoundary (0 ms)
[ RUN      ] MemoryTierManagerTest.AutoDefragmentationThreshold
[       OK ] MemoryTierManagerTest.AutoDefragmentationThreshold (0 ms)
[ RUN      ] MemoryTierManagerTest.TotalMetrics
[       OK ] MemoryTierManagerTest.TotalMetrics (0 ms)
[ RUN      ] MemoryTierManagerTest.CalculateRelationshipCoherence
[       OK ] MemoryTierManagerTest.CalculateRelationshipCoherence (0 ms)
[ RUN      ] MemoryTierManagerTest.CleanupExpiredPatterns
[       OK ] MemoryTierManagerTest.CleanupExpiredPatterns (0 ms)
[ RUN      ] MemoryTierManagerTest.PrunePatternsByPriority
[       OK ] MemoryTierManagerTest.PrunePatternsByPriority (0 ms)
[----------] 11 tests from MemoryTierManagerTest (0 ms total)

[----------] Global test environment tear-down
[==========] 11 tests from 1 test suite ran. (0 ms total)
[  PASSED  ] 8 tests.
[  FAILED  ] 3 tests, listed below:
[  FAILED  ] MemoryTierManagerTest.PromotionAndDemotion
[  FAILED  ] MemoryTierManagerTest.DefragmentationTriggersPromotionDemotion
[  FAILED  ] MemoryTierManagerTest.OptimizeBlocksPromotionDemotion

[{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "fatal_too_many_errors",
	"severity": 8,
	"message": "Too many errors emitted, stopping now",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 1,
	"endLineNumber": 1,
	"endColumn": 1
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "pp_file_not_found",
	"severity": 8,
	"message": "'core/common.h' file not found",
	"source": "clang",
	"startLineNumber": 11,
	"startColumn": 10,
	"endLineNumber": 11,
	"endColumn": 25
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'MemoryTierEnum' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 42,
	"startColumn": 14,
	"endLineNumber": 42,
	"endColumn": 28
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member_suggest",
	"severity": 8,
	"message": "No member named 'config' in namespace 'sep'; did you mean 'Config'? (fix available)",
	"source": "clang",
	"startLineNumber": 73,
	"startColumn": 38,
	"endLineNumber": 73,
	"endColumn": 43,
	"relatedInformation": [
		{
			"startLineNumber": 47,
			"startColumn": 12,
			"endLineNumber": 47,
			"endColumn": 18,
			"message": "'Config' declared here",
			"resource": "/sep/include/memory/memory_tier_manager.hpp"
		}
	]
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "typename_nested_not_found",
	"severity": 8,
	"message": "No type named 'MemoryThresholdConfig' in 'sep::memory::MemoryTierManager::Config'",
	"source": "clang",
	"startLineNumber": 73,
	"startColumn": 51,
	"endLineNumber": 73,
	"endColumn": 72
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock' (fix available)",
	"source": "clang",
	"startLineNumber": 80,
	"startColumn": 5,
	"endLineNumber": 80,
	"endColumn": 16
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 80,
	"startColumn": 45,
	"endLineNumber": 80,
	"endColumn": 59
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock' (fix available)",
	"source": "clang",
	"startLineNumber": 81,
	"startColumn": 21,
	"endLineNumber": 81,
	"endColumn": 32
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock' (fix available)",
	"source": "clang",
	"startLineNumber": 82,
	"startColumn": 5,
	"endLineNumber": 82,
	"endColumn": 16
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTier' (fix available)",
	"source": "clang",
	"startLineNumber": 85,
	"startColumn": 5,
	"endLineNumber": 85,
	"endColumn": 15
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 85,
	"startColumn": 25,
	"endLineNumber": 85,
	"endColumn": 39
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 86,
	"startColumn": 30,
	"endLineNumber": 86,
	"endColumn": 44
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 87,
	"startColumn": 32,
	"endLineNumber": 87,
	"endColumn": 46
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 90,
	"startColumn": 25,
	"endLineNumber": 90,
	"endColumn": 39
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTier' (fix available)",
	"source": "clang",
	"startLineNumber": 98,
	"startColumn": 5,
	"endLineNumber": 98,
	"endColumn": 15
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTier' (fix available)",
	"source": "clang",
	"startLineNumber": 99,
	"startColumn": 5,
	"endLineNumber": 99,
	"endColumn": 15
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTier' (fix available)",
	"source": "clang",
	"startLineNumber": 100,
	"startColumn": 5,
	"endLineNumber": 100,
	"endColumn": 15
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'SEPResult' (fix available)",
	"source": "clang",
	"startLineNumber": 103,
	"startColumn": 5,
	"endLineNumber": 103,
	"endColumn": 14
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock' (fix available)",
	"source": "clang",
	"startLineNumber": 103,
	"startColumn": 28,
	"endLineNumber": 103,
	"endColumn": 39
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock' (fix available)",
	"source": "clang",
	"startLineNumber": 103,
	"startColumn": 48,
	"endLineNumber": 103,
	"endColumn": 59
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'SEPResult' (fix available)",
	"source": "clang",
	"startLineNumber": 104,
	"startColumn": 5,
	"endLineNumber": 104,
	"endColumn": 14
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header memory is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 24,
	"startColumn": 1,
	"endLineNumber": 24,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header system_error is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 26,
	"startColumn": 1,
	"endLineNumber": 26,
	"endColumn": 24,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header unordered_map is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 27,
	"startColumn": 1,
	"endLineNumber": 27,
	"endColumn": 25,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier_manager.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header vector is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 28,
	"startColumn": 1,
	"endLineNumber": 28,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "typename_nested_not_found",
	"severity": 8,
	"message": "No type named 'PersistentPatternData' in namespace 'sep::memory::persistence'",
	"source": "clang",
	"startLineNumber": 72,
	"startColumn": 59,
	"endLineNumber": 72,
	"endColumn": 80
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'PersistentPatternData'",
	"source": "clang",
	"startLineNumber": 115,
	"startColumn": 33,
	"endLineNumber": 115,
	"endColumn": 54
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'PersistentPatternData'",
	"source": "clang",
	"startLineNumber": 116,
	"startColumn": 32,
	"endLineNumber": 116,
	"endColumn": 53
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'PersistentPatternData'",
	"source": "clang",
	"startLineNumber": 118,
	"startColumn": 11,
	"endLineNumber": 118,
	"endColumn": 32
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'PersistentPatternData'",
	"source": "clang",
	"startLineNumber": 119,
	"startColumn": 5,
	"endLineNumber": 119,
	"endColumn": 26
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'PersistentPatternData'",
	"source": "clang",
	"startLineNumber": 120,
	"startColumn": 38,
	"endLineNumber": 120,
	"endColumn": 59
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'PersistentPatternData'",
	"source": "clang",
	"startLineNumber": 138,
	"startColumn": 32,
	"endLineNumber": 138,
	"endColumn": 53
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header chrono is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 6,
	"startColumn": 1,
	"endLineNumber": 6,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header unordered_map is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 9,
	"startColumn": 1,
	"endLineNumber": 9,
	"endColumn": 25,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header vector is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 10,
	"startColumn": 1,
	"endLineNumber": 10,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/include/memory/memory_tier.hpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header shim.h is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 16,
	"startColumn": 1,
	"endLineNumber": 16,
	"endColumn": 28,
	"tags": [
		1
	]
},{
	"resource": "/sep/sep_build/src/core/logging/manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "pp_file_not_found",
	"severity": 8,
	"message": "'logging/manager.hpp' file not found",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 10,
	"endLineNumber": 1,
	"endColumn": 31
},{
	"resource": "/sep/sep_build/src/core/logging/manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'Logger'",
	"source": "clang",
	"startLineNumber": 5,
	"startColumn": 1,
	"endLineNumber": 5,
	"endColumn": 7
},{
	"resource": "/sep/sep_build/src/core/logging/manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'Manager'",
	"source": "clang",
	"startLineNumber": 5,
	"startColumn": 9,
	"endLineNumber": 5,
	"endColumn": 16
},{
	"resource": "/sep/sep_build/src/core/logging/manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'std' (fix available)",
	"source": "clang",
	"startLineNumber": 5,
	"startColumn": 34,
	"endLineNumber": 5,
	"endColumn": 37
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "fatal_too_many_errors",
	"severity": 8,
	"message": "Too many errors emitted, stopping now",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 1,
	"endLineNumber": 1,
	"endColumn": 1
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "pp_file_not_found",
	"severity": 8,
	"message": "'memory/memory_tier_manager.hpp' file not found",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 10,
	"endLineNumber": 1,
	"endColumn": 42
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 22,
	"startColumn": 16,
	"endLineNumber": 22,
	"endColumn": 33
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'core' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 23,
	"startColumn": 14,
	"endLineNumber": 23,
	"endColumn": 18
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'MemoryTierEnum' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 24,
	"startColumn": 14,
	"endLineNumber": 24,
	"endColumn": 28
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'pattern' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 25,
	"startColumn": 14,
	"endLineNumber": 25,
	"endColumn": 21
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'persistence' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 26,
	"startColumn": 14,
	"endLineNumber": 26,
	"endColumn": 25
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'quantum' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 27,
	"startColumn": 14,
	"endLineNumber": 27,
	"endColumn": 21
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 37,
	"startColumn": 17,
	"endLineNumber": 37,
	"endColumn": 34
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 37,
	"startColumn": 36,
	"endLineNumber": 37,
	"endColumn": 53
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 38,
	"startColumn": 16,
	"endLineNumber": 38,
	"endColumn": 33
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 40,
	"startColumn": 6,
	"endLineNumber": 40,
	"endColumn": 23
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 51,
	"startColumn": 1,
	"endLineNumber": 51,
	"endColumn": 18
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 51,
	"startColumn": 20,
	"endLineNumber": 51,
	"endColumn": 37
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 73,
	"startColumn": 1,
	"endLineNumber": 73,
	"endColumn": 18
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 78,
	"startColumn": 1,
	"endLineNumber": 78,
	"endColumn": 18
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'Config'",
	"source": "clang",
	"startLineNumber": 78,
	"startColumn": 44,
	"endLineNumber": 78,
	"endColumn": 50
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 82,
	"startColumn": 1,
	"endLineNumber": 82,
	"endColumn": 18
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'config' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 83,
	"startColumn": 16,
	"endLineNumber": 83,
	"endColumn": 22
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 95,
	"startColumn": 1,
	"endLineNumber": 95,
	"endColumn": 18
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 99,
	"startColumn": 6,
	"endLineNumber": 99,
	"endColumn": 23
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header algorithm is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 7,
	"startColumn": 1,
	"endLineNumber": 7,
	"endColumn": 21,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header memory is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 8,
	"startColumn": 1,
	"endLineNumber": 8,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header string is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 10,
	"startColumn": 1,
	"endLineNumber": 10,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header cstring is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 11,
	"startColumn": 1,
	"endLineNumber": 11,
	"endColumn": 19,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier_manager.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header cmath is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 12,
	"startColumn": 1,
	"endLineNumber": 12,
	"endColumn": 17,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "fatal_too_many_errors",
	"severity": 8,
	"message": "Too many errors emitted, stopping now",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 1,
	"endLineNumber": 1,
	"endColumn": 1
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "pp_file_not_found",
	"severity": 8,
	"message": "'memory/memory_tier.hpp' file not found",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 10,
	"endLineNumber": 1,
	"endColumn": 34
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'MemoryTierEnum' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 39,
	"startColumn": 14,
	"endLineNumber": 39,
	"endColumn": 28
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 41,
	"startColumn": 1,
	"endLineNumber": 41,
	"endColumn": 11
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'Config'",
	"source": "clang",
	"startLineNumber": 41,
	"startColumn": 30,
	"endLineNumber": 41,
	"endColumn": 36
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 87,
	"startColumn": 1,
	"endLineNumber": 87,
	"endColumn": 11
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 87,
	"startColumn": 24,
	"endLineNumber": 87,
	"endColumn": 38
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 93,
	"startColumn": 1,
	"endLineNumber": 93,
	"endColumn": 11
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'Config'",
	"source": "clang",
	"startLineNumber": 93,
	"startColumn": 30,
	"endLineNumber": 93,
	"endColumn": 36
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 103,
	"startColumn": 1,
	"endLineNumber": 103,
	"endColumn": 11
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock'",
	"source": "clang",
	"startLineNumber": 121,
	"startColumn": 1,
	"endLineNumber": 121,
	"endColumn": 12
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 121,
	"startColumn": 14,
	"endLineNumber": 121,
	"endColumn": 24
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 170,
	"startColumn": 6,
	"endLineNumber": 170,
	"endColumn": 16
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock'",
	"source": "clang",
	"startLineNumber": 170,
	"startColumn": 29,
	"endLineNumber": 170,
	"endColumn": 40
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "typename_nested_not_found",
	"severity": 8,
	"message": "No type named 'SEPResult' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 197,
	"startColumn": 6,
	"endLineNumber": 197,
	"endColumn": 15
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 197,
	"startColumn": 16,
	"endLineNumber": 197,
	"endColumn": 26
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 274,
	"startColumn": 7,
	"endLineNumber": 274,
	"endColumn": 17
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 300,
	"startColumn": 7,
	"endLineNumber": 300,
	"endColumn": 17
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 308,
	"startColumn": 13,
	"endLineNumber": 308,
	"endColumn": 23
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTier'",
	"source": "clang",
	"startLineNumber": 312,
	"startColumn": 13,
	"endLineNumber": 312,
	"endColumn": 23
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryBlock'",
	"source": "clang",
	"startLineNumber": 322,
	"startColumn": 18,
	"endLineNumber": 322,
	"endColumn": 29
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header algorithm is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 14,
	"startColumn": 1,
	"endLineNumber": 14,
	"endColumn": 21,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header cstdlib is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 16,
	"startColumn": 1,
	"endLineNumber": 16,
	"endColumn": 19,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header cstring is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 17,
	"startColumn": 1,
	"endLineNumber": 17,
	"endColumn": 19,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header deque is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 18,
	"startColumn": 1,
	"endLineNumber": 18,
	"endColumn": 17,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header stdexcept is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 19,
	"startColumn": 1,
	"endLineNumber": 19,
	"endColumn": 21,
	"tags": [
		1
	]
},{
	"resource": "/sep/src/memory/memory_tier.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": {
		"value": "unused-includes",
		"target": {
			"$mid": 1,
			"path": "/guides/include-cleaner",
			"scheme": "https",
			"authority": "clangd.llvm.org"
		}
	},
	"severity": 4,
	"message": "Included header vector is not used directly (fixes available)",
	"source": "clangd",
	"startLineNumber": 20,
	"startColumn": 1,
	"endLineNumber": 20,
	"endColumn": 18,
	"tags": [
		1
	]
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "fatal_too_many_errors",
	"severity": 8,
	"message": "Too many errors emitted, stopping now",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 1,
	"endLineNumber": 1,
	"endColumn": 1
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "pp_file_not_found",
	"severity": 8,
	"message": "'memory/memory_tier_manager.hpp' file not found",
	"source": "clang",
	"startLineNumber": 2,
	"startColumn": 10,
	"endLineNumber": 2,
	"endColumn": 42
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member",
	"severity": 8,
	"message": "No member named 'MemoryTierEnum' in namespace 'sep'",
	"source": "clang",
	"startLineNumber": 10,
	"startColumn": 12,
	"endLineNumber": 10,
	"endColumn": 26
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 13,
	"startColumn": 5,
	"endLineNumber": 13,
	"endColumn": 22
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 14,
	"startColumn": 36,
	"endLineNumber": 14,
	"endColumn": 50
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 15,
	"startColumn": 36,
	"endLineNumber": 15,
	"endColumn": 50
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 16,
	"startColumn": 36,
	"endLineNumber": 16,
	"endColumn": 50
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 20,
	"startColumn": 5,
	"endLineNumber": 20,
	"endColumn": 22
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock'",
	"source": "clang",
	"startLineNumber": 21,
	"startColumn": 5,
	"endLineNumber": 21,
	"endColumn": 16
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 21,
	"startColumn": 45,
	"endLineNumber": 21,
	"endColumn": 59
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 23,
	"startColumn": 38,
	"endLineNumber": 23,
	"endColumn": 52
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 26,
	"startColumn": 38,
	"endLineNumber": 26,
	"endColumn": 52
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 27,
	"startColumn": 79,
	"endLineNumber": 27,
	"endColumn": 93
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryTierManager'",
	"source": "clang",
	"startLineNumber": 32,
	"startColumn": 5,
	"endLineNumber": 32,
	"endColumn": 22
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock'",
	"source": "clang",
	"startLineNumber": 36,
	"startColumn": 5,
	"endLineNumber": 36,
	"endColumn": 16
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 36,
	"startColumn": 45,
	"endLineNumber": 36,
	"endColumn": 59
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 40,
	"startColumn": 45,
	"endLineNumber": 40,
	"endColumn": 59
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 41,
	"startColumn": 45,
	"endLineNumber": 41,
	"endColumn": 59
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'MemoryBlock'",
	"source": "clang",
	"startLineNumber": 46,
	"startColumn": 5,
	"endLineNumber": 46,
	"endColumn": 16
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 48,
	"startColumn": 31,
	"endLineNumber": 48,
	"endColumn": 45
},{
	"resource": "/sep/tests/memory/memory_tier_manager_test.cpp",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'MemoryTierEnum'",
	"source": "clang",
	"startLineNumber": 51,
	"startColumn": 39,
	"endLineNumber": 51,
	"endColumn": 53
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "fatal_too_many_errors",
	"severity": 8,
	"message": "Too many errors emitted, stopping now",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 1,
	"endLineNumber": 1,
	"endColumn": 1
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member_template",
	"severity": 8,
	"message": "No template named 'less' in namespace 'std' (fix available)",
	"source": "clang",
	"startLineNumber": 103,
	"startColumn": 67,
	"endLineNumber": 103,
	"endColumn": 71
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member_template",
	"severity": 8,
	"message": "No template named 'allocator' in namespace 'std' (fix available)",
	"source": "clang",
	"startLineNumber": 104,
	"startColumn": 29,
	"endLineNumber": 104,
	"endColumn": 38
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member_template_suggest",
	"severity": 8,
	"message": "No template named 'binary_function' in namespace 'std'; did you mean 'is_function'? (fixes available)",
	"source": "clang",
	"startLineNumber": 137,
	"startColumn": 21,
	"endLineNumber": 137,
	"endColumn": 36,
	"relatedInformation": [
		{
			"startLineNumber": 661,
			"startColumn": 12,
			"endLineNumber": 661,
			"endColumn": 23,
			"message": "'is_function' declared here",
			"resource": "/usr/include/c++/15/type_traits"
		}
	]
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "template_arg_list_different_arity",
	"severity": 8,
	"message": "Too many template arguments for class template 'is_function'",
	"source": "clang",
	"startLineNumber": 137,
	"startColumn": 21,
	"endLineNumber": 137,
	"endColumn": 36,
	"relatedInformation": [
		{
			"startLineNumber": 661,
			"startColumn": 12,
			"endLineNumber": 661,
			"endColumn": 23,
			"message": "Template is declared here",
			"resource": "/usr/include/c++/15/type_traits"
		}
	]
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "typename_nested_not_found",
	"severity": 8,
	"message": "No type named '__alloc_traits' in namespace '__gnu_cxx'",
	"source": "clang",
	"startLineNumber": 154,
	"startColumn": 15,
	"endLineNumber": 154,
	"endColumn": 49
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_member_name_or_semi",
	"severity": 8,
	"message": "Expected member name or ';' after declaration specifiers",
	"source": "clang",
	"startLineNumber": 154,
	"startColumn": 49,
	"endLineNumber": 154,
	"endColumn": 50
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier '_Select1st'",
	"source": "clang",
	"startLineNumber": 157,
	"startColumn": 46,
	"endLineNumber": 157,
	"endColumn": 56
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "no_member_template",
	"severity": 8,
	"message": "No template named '__alloc_traits' in namespace '__gnu_cxx'",
	"source": "clang",
	"startLineNumber": 163,
	"startColumn": 26,
	"endLineNumber": 163,
	"endColumn": 40
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier '_Pair_alloc_type'",
	"source": "clang",
	"startLineNumber": 163,
	"startColumn": 41,
	"endLineNumber": 163,
	"endColumn": 57
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Alloc_traits' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 175,
	"startColumn": 24,
	"endLineNumber": 175,
	"endColumn": 37
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Alloc_traits' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 176,
	"startColumn": 24,
	"endLineNumber": 176,
	"endColumn": 37
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Alloc_traits' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 177,
	"startColumn": 24,
	"endLineNumber": 177,
	"endColumn": 37
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Alloc_traits' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 178,
	"startColumn": 24,
	"endLineNumber": 178,
	"endColumn": 37
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Rep_type' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 179,
	"startColumn": 24,
	"endLineNumber": 179,
	"endColumn": 33
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Rep_type' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 180,
	"startColumn": 24,
	"endLineNumber": 180,
	"endColumn": 33
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Rep_type' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 181,
	"startColumn": 24,
	"endLineNumber": 181,
	"endColumn": 33
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Rep_type' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 182,
	"startColumn": 24,
	"endLineNumber": 182,
	"endColumn": 33
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Rep_type' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 183,
	"startColumn": 24,
	"endLineNumber": 183,
	"endColumn": 33
},{
	"resource": "/usr/include/c++/15/bits/stl_map.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_or_namespace",
	"severity": 8,
	"message": "'std::map::_Rep_type' (aka 'int') is not a class, namespace, or enumeration",
	"source": "clang",
	"startLineNumber": 184,
	"startColumn": 24,
	"endLineNumber": 184,
	"endColumn": 33
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "fatal_too_many_errors",
	"severity": 8,
	"message": "Too many errors emitted, stopping now",
	"source": "clang",
	"startLineNumber": 1,
	"startColumn": 1,
	"endLineNumber": 1,
	"endColumn": 1
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "typename_nested_not_found",
	"severity": 8,
	"message": "No type named '__alloc_traits' in namespace '__gnu_cxx'",
	"source": "clang",
	"startLineNumber": 93,
	"startColumn": 15,
	"endLineNumber": 93,
	"endColumn": 49
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_member_name_or_semi",
	"severity": 8,
	"message": "Expected member name or ';' after declaration specifiers",
	"source": "clang",
	"startLineNumber": 93,
	"startColumn": 49,
	"endLineNumber": 93,
	"endColumn": 50
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "typename_nested_not_found",
	"severity": 8,
	"message": "No type named '__alloc_traits' in namespace '__gnu_cxx'",
	"source": "clang",
	"startLineNumber": 95,
	"startColumn": 15,
	"endLineNumber": 95,
	"endColumn": 49
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_member_name_or_semi",
	"severity": 8,
	"message": "Expected member name or ';' after declaration specifiers",
	"source": "clang",
	"startLineNumber": 95,
	"startColumn": 49,
	"endLineNumber": 95,
	"endColumn": 50
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'pointer'",
	"source": "clang",
	"startLineNumber": 100,
	"startColumn": 2,
	"endLineNumber": 100,
	"endColumn": 9
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'pointer'",
	"source": "clang",
	"startLineNumber": 101,
	"startColumn": 2,
	"endLineNumber": 101,
	"endColumn": 9
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'pointer'",
	"source": "clang",
	"startLineNumber": 102,
	"startColumn": 2,
	"endLineNumber": 102,
	"endColumn": 9
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier 'pointer'",
	"source": "clang",
	"startLineNumber": 114,
	"startColumn": 59,
	"endLineNumber": 114,
	"endColumn": 66
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "expected_class_name",
	"severity": 8,
	"message": "Expected class name",
	"source": "clang",
	"startLineNumber": 140,
	"startColumn": 11,
	"endLineNumber": 140,
	"endColumn": 25
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "undeclared_var_use",
	"severity": 8,
	"message": "Use of undeclared identifier '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 144,
	"startColumn": 39,
	"endLineNumber": 144,
	"endColumn": 53
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "mem_init_not_member_or_class",
	"severity": 8,
	"message": "Member initializer '_Tp_alloc_type' does not name a non-static data member or base class",
	"source": "clang",
	"startLineNumber": 148,
	"startColumn": 4,
	"endLineNumber": 148,
	"endColumn": 20
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 152,
	"startColumn": 15,
	"endLineNumber": 152,
	"endColumn": 29
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 165,
	"startColumn": 15,
	"endLineNumber": 165,
	"endColumn": 29
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 170,
	"startColumn": 15,
	"endLineNumber": 170,
	"endColumn": 29
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 306,
	"startColumn": 7,
	"endLineNumber": 306,
	"endColumn": 21
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 311,
	"startColumn": 13,
	"endLineNumber": 311,
	"endColumn": 27
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name '_Tp_alloc_type'",
	"source": "clang",
	"startLineNumber": 349,
	"startColumn": 20,
	"endLineNumber": 349,
	"endColumn": 34
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'pointer'",
	"source": "clang",
	"startLineNumber": 383,
	"startColumn": 7,
	"endLineNumber": 383,
	"endColumn": 14
},{
	"resource": "/usr/include/c++/15/bits/stl_vector.h",
	"owner": "_generated_diagnostic_collection_name_#1",
	"code": "unknown_typename",
	"severity": 8,
	"message": "Unknown type name 'pointer'",
	"source": "clang",
	"startLineNumber": 392,
	"startColumn": 21,
	"endLineNumber": 392,
	"endColumn": 28
}]