#include "Registry.h"
#include "Results.h"
#include "ScenarioRegistry.h"
#include "../game_access/State.h"
#include "../../../Global.h"

namespace TestFramework
{
    // Helper function to report test results
    static void reportTestResults(const std::string& testName, const Test& test)
    {
        const TestResultTracker& tracker = test.getTracker();
        TestResult result;
        result.name = testName;
        result.passed = (tracker.getFailCount() == 0);
        result.duration_ms = tracker.getDuration();
        result.assertions_passed = tracker.getPassCount();
        result.assertions_failed = tracker.getFailCount();
        ResultsAggregator::getInstance().addTestResult(result);

        // Also log summary to game log
        hs_log_file("\n[%s] Tests: %d passed, %d failed (%.2fs)\n",
            testName.c_str(), tracker.getPassCount(), tracker.getFailCount(), tracker.getDuration() / 1000.0f);
    }
    Registry::Registry()
        : state(State::IDLE), currentTestIndex(0), currentTest(nullptr),
          currentStages(nullptr), waitCounter(0)
    {
    }

    Registry& Registry::getInstance()
    {
        static Registry instance;
        return instance;
    }

    void Registry::registerTest(const std::string& name, TestFunc func, const std::string& scenarioName, const ScenarioParams& scenarioParams)
    {
        tests.push_back({name, func, scenarioName, scenarioParams});
    }

    void Registry::startTests()
    {
        hs_log_file("\n========================================\n");
        hs_log_file("  STARTING TEST SUITE\n");
        hs_log_file(("  Tests: " + std::to_string(tests.size()) + "\n").c_str());
        hs_log_file("========================================\n\n");

        state = State::INITIALIZING_TEST;
        currentTestIndex = 0;
        currentTest = nullptr;
        currentStages = nullptr;
        waitCounter = 0;
    }

    void Registry::tick()
    {
        if (state == State::IDLE || state == State::COMPLETE) return;

        switch (state)
        {
            case State::INITIALIZING_TEST:
            {
                if (currentTestIndex >= tests.size())
                {
                    // All tests complete
                    ResultsAggregator::getInstance().writeResults();

                    auto& aggregator = ResultsAggregator::getInstance();
                    hs_log_file("\n========================================\n");
                    hs_log_file("  TEST SUITE COMPLETE\n");
                    hs_log_file(("  Total:  " + std::to_string(aggregator.getTotalTests()) + "\n").c_str());
                    hs_log_file(("  Passed: " + std::to_string(aggregator.getPassedTests()) + "\n").c_str());
                    hs_log_file(("  Failed: " + std::to_string(aggregator.getFailedTests()) + "\n").c_str());
                    hs_log_file("========================================\n");

                    state = State::COMPLETE;
                    break;
                }

                const auto& testInfo = tests[currentTestIndex];

                // Create test and stages
                currentTest = new Test(testInfo.name);
                currentStages = new TestStages(currentTest);

                try
                {
                    // Setup scenario first
                    ScenarioRegistry::getInstance().setupScenario(
                        testInfo.scenarioName,
                        currentTest,
                        currentStages,
                        testInfo.scenarioParams
                    );

                    // Call test function to set up test-specific stages
                    testInfo.func(*currentTest, *currentStages);
                    state = State::RUNNING_STAGES;
                }
                catch (const TestFailedException& e)
                {
                    currentTest->log("Test failed during setup: " + std::string(e.what()));
                    reportTestResults(testInfo.name, *currentTest);
                    delete currentStages;
                    delete currentTest;
                    currentTestIndex++;
                    state = State::WAITING_BETWEEN_TESTS;
                    waitCounter = 60; // Wait 1 second before next test
                }
                catch (const std::exception& e)
                {
                    currentTest->fail("Test exception during setup", e.what());
                    reportTestResults(testInfo.name, *currentTest);
                    delete currentStages;
                    delete currentTest;
                    currentTestIndex++;
                    state = State::WAITING_BETWEEN_TESTS;
                    waitCounter = 60;
                }
                catch (...)
                {
                    currentTest->fail("Unknown exception during setup", "Test threw non-standard exception");
                    reportTestResults(testInfo.name, *currentTest);
                    delete currentStages;
                    delete currentTest;
                    currentTestIndex++;
                    state = State::WAITING_BETWEEN_TESTS;
                    waitCounter = 60;
                }
                break;
            }

            case State::RUNNING_STAGES:
            {
                const auto& testInfo = tests[currentTestIndex];
                try
                {
                    bool complete = currentStages->tick();
                    if (complete)
                    {
                        // Test stages complete - report results
                        reportTestResults(testInfo.name, *currentTest);
                        delete currentStages;
                        delete currentTest;
                        currentTestIndex++;

                        // Return to main menu between tests for isolation
                        if (currentTestIndex < tests.size())
                        {
                            hs_log_file("\n[TEST ISOLATION] Returning to main menu...\n");
                            GameAccess::State::returnToMainMenu();
                            state = State::WAITING_BETWEEN_TESTS;
                            waitCounter = 60; // Wait 1 second before next test
                        }
                        else
                        {
                            state = State::INITIALIZING_TEST;
                        }
                    }
                }
                catch (const TestFailedException& e)
                {
                    currentTest->log("Test failed: " + std::string(e.what()));
                    reportTestResults(testInfo.name, *currentTest);
                    delete currentStages;
                    delete currentTest;
                    currentTestIndex++;
                    state = State::WAITING_BETWEEN_TESTS;
                    waitCounter = 60;
                }
                catch (const std::exception& e)
                {
                    currentTest->fail("Test exception", e.what());
                    reportTestResults(testInfo.name, *currentTest);
                    delete currentStages;
                    delete currentTest;
                    currentTestIndex++;
                    state = State::WAITING_BETWEEN_TESTS;
                    waitCounter = 60;
                }
                catch (...)
                {
                    currentTest->fail("Unknown exception", "Test threw non-standard exception");
                    reportTestResults(testInfo.name, *currentTest);
                    delete currentStages;
                    delete currentTest;
                    currentTestIndex++;
                    state = State::WAITING_BETWEEN_TESTS;
                    waitCounter = 60;
                }
                break;
            }

            case State::WAITING_BETWEEN_TESTS:
            {
                if (waitCounter > 0)
                {
                    if (waitCounter % 60*2 == 0) // Log every 2 seconds
                    {
                        hs_log_file(("[TEST ISOLATION] Waiting... " + std::to_string(waitCounter / 60) + " seconds remaining\n").c_str());
                    }
                    waitCounter--;
                }
                else
                {
                    state = State::INITIALIZING_TEST;
                }
                break;
            }

            default:
                break;
        }
    }

    bool Registry::isComplete() const
    {
        return state == State::COMPLETE;
    }
}
