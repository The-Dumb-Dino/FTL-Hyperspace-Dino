#pragma once
#include <string>
#include <vector>

/**
 * TestFramework::Results
 *
 * Test result aggregation and reporting.
 * Collects results from all tests and writes machine-readable JSON output.
 */

namespace TestFramework
{
    /**
     * Result data for a single test
     */
    struct TestResult
    {
        std::string name;
        bool passed;
        int duration_ms;
        int assertions_passed;
        int assertions_failed;
    };

    /**
     * Aggregates and reports test results
     * Singleton pattern for global access
     */
    class ResultsAggregator
    {
    public:
        static ResultsAggregator& getInstance();

        // Add a test result
        void addTestResult(const TestResult& result);

        // Write results to human-readable text file
        void writeResults();

        // Query methods (cached)
        bool allPassed() const { return passedCount == static_cast<int>(results.size()); }
        int getTotalTests() const { return static_cast<int>(results.size()); }
        int getPassedTests() const { return passedCount; }
        int getFailedTests() const { return getTotalTests() - passedCount; }

    private:
        ResultsAggregator() = default;

        std::vector<TestResult> results;
        int passedCount = 0;  // Cached to avoid O(n) loops
    };
}
